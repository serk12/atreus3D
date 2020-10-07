#include "meshTypes.h"

#include <iostream>

// ******* //
// GENERAL //
// ******* //

void calculateNormal(const Eigen::Vector3f& A, const Eigen::Vector3f& B, const Eigen::Vector3f& C, Eigen::Vector3f& n, float& d)
{
    Eigen::Vector3f AB = B - A;
    Eigen::Vector3f AC = C - A;

    n = (AB.cross(AC)).normalized();
    d = -(n.x()*A.x() + n.y()*A.y() + n.z()*A.z());

    if (d <= 0.0f) {
        d = -d;
        n = -1.0f*n;
    }
}

inline bool planeCrossed(const Eigen::Vector3f& n, const Eigen::Vector3f& p, const Eigen::Vector3f& p_pass, const float d)
{
    return (n.dot(p) + d) * (n.dot(p_pass) + d) <= 0.0f;
}

void correctParticle(Eigen::Vector3f& p, Eigen::Vector3f& v, Eigen::Vector3f& p_pass, const float e, const Eigen::Vector3f& n, const float d)
{
    v = v - (1.0f + e) * (n.dot(v)) * n;
    p = p - (1.0f + e) * (n.dot(p)+d) * n;
    p_pass = p - (v * 0.016f);
}

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

bool Sphere::isColliding(Eigen::Vector3f& p, Eigen::Vector3f& p_pass, Eigen::Vector3f& v, const float r) const
{
    Eigen::Vector3f diff_p = p - this->p;
    if ((diff_p).transpose() * (diff_p) - (this->r*this->r) <= 0.01f) {
        Eigen::Vector3f n = diff_p.normalized();
        float d = -(n.x()*p.x() + n.y()*p.y() + n.z()*p.z());
        correctParticle(p, v, p_pass, 0.95f, n, d);
    }
    return false;
}

// ******** //
// TRIANGLE //
// ******** //

inline float areaTrangle(const Eigen::Vector3f& A, const Eigen::Vector3f& B, const Eigen::Vector3f& C)
{
    Eigen::Vector3f BA = (B - A);
    Eigen::Vector3f CA = (C - A);

    return (BA.cross(CA)).norm()*0.5f;
}

Triangle::Triangle() : Mesh() {}
Triangle::Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, GL_TRIANGLES)
{
    this->A = Eigen::Vector3f(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]) + p;
    this->B = Eigen::Vector3f(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]) + p;
    this->C = Eigen::Vector3f(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]) + p;

    calculateNormal(A,B,C, n, d);
    area = areaTrangle(A, B, C);
}

Triangle::Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, type)
{
    this->A = Eigen::Vector3f(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]) + p;
    this->B = Eigen::Vector3f(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]) + p;
    this->C = Eigen::Vector3f(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]) + p;

    calculateNormal(A ,B, C, n, d);
    area = areaTrangle(A, B, C);
}


float Triangle::getRadius() const
{
    return 1;
}

bool Triangle::isColliding(Eigen::Vector3f& p, Eigen::Vector3f& p_pass, Eigen::Vector3f& v, const float r) const
{
    if(planeCrossed(n, p, p_pass, d) and (areaTrangle(p, B, C) + areaTrangle(A, p, C) + areaTrangle(A, B, p) - area <= 0.1f)) {
        correctParticle(p, v, p_pass, 0.95f, n, d);
        return true;
    }
    return false;
}

// ***** //
// PLANE //
// ***** //


Plane::Plane() : Mesh() {}
Plane::Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, GL_TRIANGLES)
{
    Eigen::Vector3f A = Eigen::Vector3f(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]);
    Eigen::Vector3f B = Eigen::Vector3f(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]);
    Eigen::Vector3f C = Eigen::Vector3f(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]);

    calculateNormal(A, B, C, n, d);
}


Plane::Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, type)
{
    Eigen::Vector3f A = Eigen::Vector3f(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]);
    Eigen::Vector3f B = Eigen::Vector3f(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]);
    Eigen::Vector3f C = Eigen::Vector3f(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]);

    calculateNormal(A, B, C, n, d);
}


float Plane::getRadius() const
{
    return 1;
}

bool Plane::isColliding(Eigen::Vector3f& p, Eigen::Vector3f& p_pass, Eigen::Vector3f& v, const float r) const
{
    if(planeCrossed(n, p, p_pass, d)) {
        correctParticle(p, v, p_pass, 0.95f, n, d);
        return true;
    }
    return false;
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

bool Polygon::isColliding(Eigen::Vector3f&, Eigen::Vector3f&, Eigen::Vector3f&, const float) const
{
    return false;
}
