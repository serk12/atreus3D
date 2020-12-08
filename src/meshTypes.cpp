#include "meshTypes.h"

#include <iostream>

// ******* //
// GENERAL //
// ******* //

void calculateNormal(const Eigen::Vector3d& A, const Eigen::Vector3d& B, const Eigen::Vector3d& C, Eigen::Vector3d& n, double& d)
{
    Eigen::Vector3d AB = B - A;
    Eigen::Vector3d AC = C - A;

    n = (AB.cross(AC)).normalized();
    d = -(n.x()*A.x() + n.y()*A.y() + n.z()*A.z());

    if (d <= 0.0f) {
        d = -d;
        n = -1.0f*n;
    }
}

inline bool planeCrossed(const Eigen::Vector3d& n, const double d, const Eigen::Vector3d& p, const Eigen::Vector3d& p_pass, const double r = 0.0f)
{
    double d_aux = d - (r*n).norm();
    return (n.dot(p) + d_aux) * (n.dot(p_pass) + d_aux) <= 0.0f;
}

// ****** //
// SPHERE //
// ****** //

Sphere::Sphere() : Mesh() {}
Sphere::Sphere(const std::vector<double> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3d(0.5f, 1.0f, 0.5f), p, v, m, 0.95, 0.30f, GL_TRIANGLES) {}

Sphere::Sphere(const std::vector<double> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u, const double r)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, GL_POINTS)
{
    this->r = r;
    this->r2 = r*r;
}

double Sphere::getRadius() const
{
    return r;
}

bool Sphere::isColliding(Object &object) const
{
    if (physicsType == PhysicsType::Transparent) return false;
    Eigen::Vector3d p = object.getPosition();
    Eigen::Vector3d v = object.getVelocity();
    double r2o = object.getRadius() * object.getRadius();
    Eigen::Vector3d c = this->p;
    Eigen::Vector3d diff_p = p - c;
    if ((diff_p.transpose() * diff_p) - (this->r2 + r2o) <= 0) {
        double alpha = v.dot(v), beta = (2*v).dot(p-c), gamma = c.dot(c) + p.dot(p)-(2*p).dot(c) - r2;
        double aux_1 = sqrt(beta*beta-4*alpha*gamma), aux_2 = 2*alpha;
        double dir1 = (-beta+aux_1)/(aux_2);
        double dir2 = (-beta-aux_1)/(aux_2);
        if (std::isnan(dir1) && std::isnan(dir2)) return false;
        Eigen::Vector3d P = object.getPosition() + ((dir1 >= 0.0f)? dir2 : dir1)*v;
        Eigen::Vector3d n = (P - c).normalized();
        double d = -(n.x()*P.x() + n.y()*P.y() + n.z()*P.z());
        object.correctObject(n, d, true);
    }
    return false;
}

// ******** //
// TRIANGLE //
// ******** //

inline double areaTrangle(const Eigen::Vector3d& A, const Eigen::Vector3d& B, const Eigen::Vector3d& C)
{
    Eigen::Vector3d BA = (B - A);
    Eigen::Vector3d CA = (C - A);

    return (BA.cross(CA)).norm()*0.5f;
}

Triangle::Triangle() : Mesh() {}
Triangle::Triangle(const std::vector<double> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3d(0.5f, 1.0f, 0.5f), p, v, m, 0.95, 0.60f, GL_TRIANGLES)
{
    this->A = Eigen::Vector3d(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]) + p;
    this->B = Eigen::Vector3d(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]) + p;
    this->C = Eigen::Vector3d(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]) + p;

    calculateNormal(A,B,C, n, d);
    area = areaTrangle(A, B, C);
}

Triangle::Triangle(const std::vector<double> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, type)
{
    this->A = Eigen::Vector3d(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]) + p;
    this->B = Eigen::Vector3d(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]) + p;
    this->C = Eigen::Vector3d(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]) + p;

    calculateNormal(A ,B, C, n, d);
    area = areaTrangle(A, B, C);
}


double Triangle::getRadius() const
{
    return 1;
}

bool Triangle::isColliding(Object &object) const
{
    if (physicsType == PhysicsType::Transparent) return false;
    if (planeCrossed(n, d, object.getPosition(), object.getPassPosition(), object.getRadius()) and (areaTrangle(object.getPosition(), B, C) + areaTrangle(A, object.getPosition(), C) + areaTrangle(A, B, object.getPosition()) - area <= 0.01f)) {
        object.correctObject(n, d, false);
        return true;
    }
    return false;
}

// ***** //
// PLANE //
// ***** //


Plane::Plane() : Mesh() {}
Plane::Plane(const std::vector<double> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3d(0.5f, 1.0f, 0.5f), p, v, m, 0.95, 0.50f, GL_TRIANGLES)
{
    Eigen::Vector3d A = Eigen::Vector3d(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]) + p;
    Eigen::Vector3d B = Eigen::Vector3d(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]) + p;
    Eigen::Vector3d C = Eigen::Vector3d(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]) + p;

    calculateNormal(A, B, C, n, d);
}


Plane::Plane(const std::vector<double> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, type)
{
    Eigen::Vector3d A = Eigen::Vector3d(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]) + p;
    Eigen::Vector3d B = Eigen::Vector3d(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]) + p;
    Eigen::Vector3d C = Eigen::Vector3d(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]) + p;

    calculateNormal(A, B, C, n, d);
}


double Plane::getRadius() const
{
    return 1;
}

bool Plane::isColliding(Object &object) const
{
    if (physicsType == PhysicsType::Transparent) return false;
    if(planeCrossed(n, d, object.getPosition(), object.getPassPosition(), object.getRadius())) {
        object.correctObject(n, d, false);
        return true;
    }
    return false;
}

// ******* //
// POLYGON //
// ******* //

Polygon::Polygon() : Mesh() {}
Polygon::Polygon(const std::vector<double> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3d(0.5f, 1.0f, 0.5f), p, v, m, 0.95f, 0.250f, GL_TRIANGLES) {}

Polygon::Polygon(const std::vector<double> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, type) {}

double Polygon::getRadius() const
{
    return 1;
}

bool Polygon::isColliding(Object &) const
{

    if (physicsType == PhysicsType::Transparent) return false;
    return false;
}
