#include <GL/glew.h>

#include <iostream>
#include "mesh.h"

Mesh::Mesh()
    : Mesh({0.0f, 0.5f, 0.5f,   0.0f, 0.5f, -0.5f,   0.0f, -0.5f, -0.5f,   0.0f, -0.5f,  0.5f},
           {0, 1, 3, 1, 2, 3, 3, 1, 0, 3, 2, 1}, ShaderType::Vanilla, Eigen::Vector3f(0.6f, 0.2f, 0.6f),
           Eigen::Vector3f(0.0f, 1.0f, 0.0f), Eigen::Vector3f(0.0f, -0.0002f, 0.0f), 1.0f, 0.95f, 0.830f, GL_TRIANGLES) {}

Mesh::Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, e, u, GL_TRIANGLES) {}

Mesh::Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const GLenum type)
{
    this->vertices = vertices;
    this->indices = indices;
    this->qttyFaces = vertices.size();
    this->shaderType = programIndice;
    this->objectColor = color;
    this->type = type;
    this->p = p;
    this->v = v;
    this->m = m;
    this->e = e;
    this->u = u;
    //predefined
    initSolver();
}

void Mesh::render() const
{
    renderType(type);
}

void Mesh::event(QEvent *) {}

void Mesh::forceUpdate() {}

void Mesh::collisionDetect(const std::list<Object*>&) {}

bool Mesh::possitionCorrect() {return false;}

void Mesh::correctParticle(const Eigen::Vector3f&, const float) {}
