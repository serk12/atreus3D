#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include "mesh.h"

const char defaultFile[] = "asd";
Mesh::Mesh() : Mesh(defaultFile) {}

Mesh::Mesh(const char[])
{

    vertices = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        };
        indices = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

    qttyFaces = sizeof(indices)/sizeof(unsigned int);

    programVars.push_back(ProgramVars::projMatrix);
    programVars.push_back(ProgramVars::lightPos);
    programVars.push_back(ProgramVars::mvMatrix);
    programVars.push_back(ProgramVars::normalMatrix);
}

void Mesh::render()
{

    glUseProgram(Object::vanilla);
    glm::vec3 lightColor(1.0f);
    glUniform3fv(glGetUniformLocation(Object::vanilla, "lightColor"), 1, &lightColor[0]);
    glm::vec3 objectColor(0.7f, 0.5f, 0.2f);
    glUniform3fv(glGetUniformLocation(Object::vanilla, "objectColor"), 1, &objectColor[0]);

    glUniformMatrix4fv(glGetUniformLocation(Object::vanilla, "view"), 1, GL_FALSE, Object::view.data());
    glUniformMatrix4fv(glGetUniformLocation(Object::vanilla, "model"), 1, GL_FALSE, Object::model.data());
    glUniformMatrix4fv(glGetUniformLocation(Object::vanilla, "projection"), 1, GL_FALSE, Object::projection.data());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}

void Mesh::update(const float) {}

void Mesh::event(QEvent *) {}

bool Mesh::load()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return true;
}
