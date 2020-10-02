#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include "mesh.h"

const char defaultFile[] = "asd";
Mesh::Mesh() : Mesh(defaultFile) {}

Mesh::Mesh(const char[])
{
    vertices = {
        0.5f,  0.5f, 0.0f,  // top right
                         0.5f, -0.5f, 0.0f,  // bottom right
                        -0.5f, -0.5f, 0.0f,  // bottom left
                        -0.5f,  0.5f, 0.0f   // top left
    };
    indices = {
        0, 1, 3, // first triangle
        1, 2, 3,  // second triangle
        3, 1, 0,
        3, 2, 1
    };

    qttyFaces = indices.size();

    programVars.push_back(ProgramVars::projMatrix);
    programVars.push_back(ProgramVars::lightPos);
    programVars.push_back(ProgramVars::mvMatrix);
    programVars.push_back(ProgramVars::normalMatrix);
}

void Mesh::update(const float) {}

void Mesh::event(QEvent *) {}

bool Mesh::load()
{
    if (program == 0) {
        program = Object::vanilla;
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return true;
}
