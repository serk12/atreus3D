#include "meshTypes.h"

// ****** //
// SPHERE //
// ****** //

Sphere::Sphere() : Mesh() {}
Sphere::Sphere(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, GL_TRIANGLES) {}

Sphere::Sphere(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, programIndice, color, p, v, m, GL_POINTS) {}

float Sphere::getRadius() const
{
    return r;
}

// ******** //
// TRIANGLE //
// ******** //

Triangle::Triangle() : Mesh() {}
Triangle::Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, GL_TRIANGLES) {}

Triangle::Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, type) {}


float Triangle::getRadius() const
{
    return 1;
}

// ***** //
// PLANE //
// ***** //

Plane::Plane() : Mesh() {}
Plane::Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, GL_TRIANGLES) {}

Plane::Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, type) {}

float Plane::getRadius() const
{
    return 1;
}

// ******* //
// POLYGON //
// ******* //

Polygon::Polygon() : Mesh() {}
Polygon::Polygon(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, GL_TRIANGLES) {}

Polygon::Polygon(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, type) {}

float Polygon::getRadius() const
{
    return 1;
}
