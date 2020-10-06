#include <GL/glew.h>

#include <iostream>
#include "mesh.h"

Mesh::Mesh()
    : Mesh({0.0f, 0.5f, 0.5f,   0.0f, 0.5f, -0.5f,   0.0f, -0.5f, -0.5f,   0.0f, -0.5f,  0.5f},
           {0, 1, 3, 1, 2, 3, 3, 1, 0, 3, 2, 1}, 0, Eigen::Vector3f(0.6f, 0.2f, 0.6f),
           Eigen::Vector3f(0.0f, 1.0f, 0.0f), Eigen::Vector3f(0.0f, -0.0002f, 0.0f), 1.0f, GL_TRIANGLES) {}

Mesh::Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, 0, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, GL_TRIANGLES) {}

Mesh::Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices, const unsigned int programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const GLenum type)
{
    this->vertices = vertices;
    this->indices = indices;
    this->qttyFaces = vertices.size();
    this->program = Object::programsList[programIndice];
    this->objectColor = color;
    this->type = type;
    this->p = p;
    this->v = v;
    this->m = m;

    //predefined
    this->objectType = ObjectType::Mesh;
    initSolver();
}

void Mesh::render() const
{
    renderType(type);
}

void Mesh::event(QEvent *) {}

void Mesh::forceUpdate() {}

void Mesh::collisionDetect() {}

bool Mesh::possitionCorrect() {return false;}
