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

inline bool planeCrossed(const Eigen::Vector3f& n, const float d, const Eigen::Vector3f& p, const Eigen::Vector3f& p_pass, const float = 0.0f)
{
    return (n.dot(p) + d) * (n.dot(p_pass) + d) <= 0.0f;
}

void correctParticle(Eigen::Vector3f& p, Eigen::Vector3f& v, Eigen::Vector3f& p_pass, const float e, const float u, const Eigen::Vector3f& n, const float d)
{
    Eigen::Vector3f vt = v - ((n.dot(v)) * n);
    v = (v - (1.0f + e) * (n.dot(v)) * n) - (u * vt);
    p = p - (1.0f + e) * (n.dot(p)+d) * n;
    p_pass = p - (v * 0.016f);
}

// ****** //
// SPHERE //
// ****** //

Sphere::Sphere() : Mesh() {}
Sphere::Sphere(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, 0.95, 0.30f, GL_TRIANGLES) {}

Sphere::Sphere(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const float r)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, GL_POINTS)
{
    this->r = r;
    this->r2 = r*r;
}

float Sphere::getRadius() const
{
    return r;
}

bool Sphere::isColliding(Object &object) const
{
    if (physicsType == PhysicsType::Transparent) return false;
    Eigen::Vector3f p = object.getPosition();
    Eigen::Vector3f v = object.getVelocity();
    Eigen::Vector3f c = this->p;
    Eigen::Vector3f diff_p = p - c;
    if ((diff_p).transpose() * (diff_p) - ((this->r*this->r) - (r*r)) <= 0.0f) {
        float alpha = v.dot(v), beta = (2*v).dot(p-c), gamma = c.dot(c) + p.dot(p)-(2*p).dot(c) - r2;
        float aux_1 = sqrt(beta*beta-4*alpha*gamma), aux_2 = 2*alpha;
        float dir1 = (-beta+aux_1)/(aux_2);
        float dir2 = (-beta-aux_1)/(aux_2);
        Eigen::Vector3f P = object.getPosition() + ((dir1 >= 0.0f)? dir2 : dir1)*v;
        Eigen::Vector3f n = (P - c).normalized();
        float d = -(n.x()*p.x() + n.y()*p.y() + n.z()*p.z());
        object.correctParticle(n, d);
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
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, 0.95, 0.60f, GL_TRIANGLES)
{
    this->A = Eigen::Vector3f(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]) + p;
    this->B = Eigen::Vector3f(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]) + p;
    this->C = Eigen::Vector3f(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]) + p;

    calculateNormal(A,B,C, n, d);
    area = areaTrangle(A, B, C);
}

Triangle::Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, type)
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

bool Triangle::isColliding(Object &object) const
{
    if (physicsType == PhysicsType::Transparent) return false;
    if (planeCrossed(n, d, object.getPosition(), object.getPassPosition(), object.getRadius()) and (areaTrangle(object.getPosition(), B, C) + areaTrangle(A, object.getPosition(), C) + areaTrangle(A, B, object.getPosition()) - area <= 0.01f)) {
        object.correctParticle(n, d);
        return true;
    }
    return false;
}

// ***** //
// PLANE //
// ***** //


Plane::Plane() : Mesh() {}
Plane::Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, 0.95, 0.50f, GL_TRIANGLES)
{
    Eigen::Vector3f A = Eigen::Vector3f(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]);
    Eigen::Vector3f B = Eigen::Vector3f(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]);
    Eigen::Vector3f C = Eigen::Vector3f(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]);

    calculateNormal(A, B, C, n, d);
}


Plane::Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, type)
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

bool Plane::isColliding(Object &object) const
{
    if (physicsType == PhysicsType::Transparent) return false;
    if(planeCrossed(n, d, object.getPosition(), object.getPassPosition(), object.getRadius())) {
        object.correctParticle(n, d);
        return true;
    }
    return false;
}

// ******* //
// POLYGON //
// ******* //

Polygon::Polygon() : Mesh() {}
Polygon::Polygon(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, 0.95f, 0.250f, GL_TRIANGLES) {}

Polygon::Polygon(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, type) {}

float Polygon::getRadius() const
{
    return 1;
}

bool Polygon::isColliding(Object &) const
{

    if (physicsType == PhysicsType::Transparent) return false;
    return false;
}
