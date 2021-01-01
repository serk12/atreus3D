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

inline bool planeCrossed(const Eigen::Vector3f& n, const float d, const Eigen::Vector3f& p, const Eigen::Vector3f& p_pass, const float r = 0.0f)
{
    float d_aux = d - (r*n).norm();
    return (n.dot(p) + d_aux) * (n.dot(p_pass) + d_aux) <= 0.0f;
}


inline bool sphereVsSphere(Eigen::Vector3f p1, Eigen::Vector3f p2, float rs1, float rs2)
{
    Eigen::Vector3f diff_p = p1 - p2;
    return (((diff_p.transpose() * diff_p) - (rs1 + rs2)) <= 0);
}

inline float areaTrangle(const Eigen::Vector3f& A, const Eigen::Vector3f& B, const Eigen::Vector3f& C)
{
    Eigen::Vector3f BA = (B - A);
    Eigen::Vector3f CA = (C - A);

    return (BA.cross(CA)).norm()*0.5f;
}

inline bool triangleCrossed(const Eigen::Vector3f& n, const float d, const float r, const Eigen::Vector3f& p, const Eigen::Vector3f& p_pass, const Eigen::Vector3f& A, const Eigen::Vector3f& B, const Eigen::Vector3f& C, const float area)
{
    return planeCrossed(n, d, p, p_pass, r) and (areaTrangle(p, B, C) + areaTrangle(A, p, C) + areaTrangle(A, B, p) - area <= 0.01f);
}


// ****** //
// SPHERE //
// ****** //

Sphere::Sphere()
    : Sphere(Eigen::Vector3f(0.2f,0.5f,1.0f), Eigen::Vector3f(0.0f,0.2f,0.1f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1, 0.95f, 0.80f, 0.5f) {}

Sphere::Sphere(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float r)
    : Sphere(Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, 0.95, 0.30f, r) {}

Sphere::Sphere(const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const float r)
    : Mesh({0.0f, 0.0f, 0.0f}, {0}, ShaderType::Ball, color, p, v, m, e, u, GL_POINTS)
{
    this->r = r;
    this->r2 = r*r;
    this->_type = Object::ObjectType::_Sphere;
}

float Sphere::getRadius() const
{
    return r;
}

float Sphere::getRadiusSqrt() const
{
    return this->r2;
}

bool Sphere::isColliding(Object &object) const
{
    if (physicsType == PhysicsType::Transparent) return false;
    Eigen::Vector3f p = object.getPosition();
    if (sphereVsSphere(p, this->p, this->r2, object.getRadiusSqrt())) {
        Eigen::Vector3f c = this->p;
        Eigen::Vector3f v = object.getVelocity();
        float alpha = v.dot(v), beta = (2*v).dot(p-c), gamma = c.dot(c) + p.dot(p)-(2*p).dot(c) - r2;
        float aux_1 = sqrt(beta*beta-4*alpha*gamma), aux_2 = 2*alpha;
        float dir1 = (-beta+aux_1)/(aux_2);
        float dir2 = (-beta-aux_1)/(aux_2);
        if (std::isnan(dir1) && std::isnan(dir2)) return false;
        Eigen::Vector3f P = object.getPosition() + ((dir1 >= 0.0f)? dir2 : dir1)*v;
        Eigen::Vector3f n = (P - c).normalized();
        float d = -(n.x()*P.x() + n.y()*P.y() + n.z()*P.z());
        object.correctObject(n, d, true);
    }
    return false;
}

// ******** //
// TRIANGLE //
// ******** //

Triangle::Triangle()
    : Triangle({0.0f, 0.5f, 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,}, {0,1,2,2,1,0,}, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), Eigen::Vector3f(0.0f,0.2f,0.1f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1, 0.95, 0.60f, GL_TRIANGLES) {}
Triangle::Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Triangle(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, 0.95, 0.60f, GL_TRIANGLES)
{}

Triangle::Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, type)
{
    this->A = Eigen::Vector3f(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]);
    this->B = Eigen::Vector3f(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]);
    this->C = Eigen::Vector3f(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]);

    calculateNormal(A ,B, C, n, d);
    area = areaTrangle(A, B, C);

    r = (A - B).norm();
    r = (B - C).norm() > r? (B - C).norm() : r;
    r = (C - A).norm() > r? (C - A).norm() : r;
    r /= 2.0f;
    r2 = r*r;
    this->_type = Object::ObjectType::_Triangle;
}


float Triangle::getRadius() const
{
    return r;
}

float Triangle::getRadiusSqrt() const
{
    return r2;
}

bool Triangle::isColliding(Object &object) const
{
    Eigen::Vector3f aux_A = A + this->p;
    Eigen::Vector3f aux_B = B + this->p;
    Eigen::Vector3f aux_C = C + this->p;
    if (physicsType == PhysicsType::Transparent) return false;
    if (planeCrossed(n, d, object.getPosition(), object.getPassPosition(), object.getRadius()) and
                    (areaTrangle(object.getPosition(), aux_B, aux_C) + areaTrangle(aux_A, object.getPosition(), aux_C) + areaTrangle(aux_A, aux_B, object.getPosition()) - area <= 0.01f)) {
        object.correctObject(n, d, false);
        return true;
    }
    return false;
}

// ***** //
// PLANE //
// ***** //


Plane::Plane()
    : Plane({-1.0f, -1.0f, 1.0f,1.0f, -1.0f, 1.0f,1.0f, -1.0f, -1.0f,-1.0f, -1.0f, -1.0f,}, {0,1,2,3}, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), Eigen::Vector3f(0.0f,0.2f,0.1f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1, 0.95, 0.50f, GL_TRIANGLES) {}
Plane::Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Plane(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, 0.95, 0.50f, GL_TRIANGLES) {}
Plane::Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const GLenum type)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, type)
{
    Eigen::Vector3f A = Eigen::Vector3f(vertices[indices[0]*3], vertices[indices[0]*3+1], vertices[indices[0]*3+2]) + p;
    Eigen::Vector3f B = Eigen::Vector3f(vertices[indices[1]*3], vertices[indices[1]*3+1], vertices[indices[1]*3+2]) + p;
    Eigen::Vector3f C = Eigen::Vector3f(vertices[indices[2]*3], vertices[indices[2]*3+1], vertices[indices[2]*3+2]) + p;

    calculateNormal(A, B, C, n, d);
    this->_type = Object::ObjectType::_Plane;
}


float Plane::getRadius() const
{
    return std::numeric_limits<float>::infinity();
}

float Plane::getRadiusSqrt() const
{
    return std::numeric_limits<float>::infinity();
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

Polygon::Polygon(const std::string& path, const Eigen::Vector3f offSet, const float scale, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Mesh(vertices, indices, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, 0.95f, 0.250f, loadModel(path, offSet, scale)) {
}

Polygon::Polygon(const std::string& path, const Eigen::Vector3f offSet, const float scale, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u)
    : Mesh(vertices, indices, programIndice, color, p, v, m, e, u, loadModel(path, offSet, scale)) {
}

GLenum Polygon::loadModel(const std::string& path, const Eigen::Vector3f offSet, const float scale)
{
    model.load(path);
    this->vertices = std::vector<float>{model.VBO_vertices(),
                                        model.VBO_vertices() + sizeof(GLfloat)*model.faces().size()*3*3};

    std::vector<Face> faces = model.faces();
    this->indices = std::vector<unsigned int>(faces.size()*3);

    for (unsigned int i = 0; i < faces.size()*3; ++i) {
        this->indices[i] = i;
    }

    maxX = minX = (vertices[0] + offSet.x()) / scale;
    maxY = minY = (vertices[1] + offSet.y()) / scale;
    maxZ = minZ = (vertices[2] + offSet.z()) / scale;
    for (unsigned int i = 0; i < vertices.size() / 3; ++i) {
        vertices[i*3+0] = (vertices[i*3+0] + offSet.x()) / scale;
        vertices[i*3+1] = (vertices[i*3+1] + offSet.y()) / scale;
        vertices[i*3+2] = (vertices[i*3+2] + offSet.z()) / scale;

        if (vertices[i*3+0] > maxX) {maxX = vertices[i*3+0];}
        if (vertices[i*3+1] > maxY) {maxY = vertices[i*3+1];}
        if (vertices[i*3+2] > maxZ) {maxZ = vertices[i*3+2];}

        if (vertices[i*3+0] < minX) {minX = vertices[i*3+0];}
        if (vertices[i*3+1] < minY) {minY = vertices[i*3+1];}
        if (vertices[i*3+2] < minZ) {minZ = vertices[i*3+2];}
    }

    r = (maxX - minX);
    float Y = (maxY - minY);
    float Z = (maxZ - minZ);
    r = (r < Y)? Y : r;
    r = (r < Z)? Z : r;
    r2 = r*r;

    this->_type = ObjectType::_Polygon;

    return GL_TRIANGLES;
}


float Polygon::getRadius() const
{
    return r;
}

float Polygon::getRadiusSqrt() const
{
    return r2;
}

inline bool polygonPoint(const Eigen::Vector3f& i, const Eigen::Vector3f& j, const Eigen::Vector3f& p)
{
    return (((i.y()>p.y()) != (j.y()>p.y())) &&
            (p.x() < (j.x()-i.x()) * (p.y()-i.y()) / (j.y()-i.y()) + i.x()));
}

int Polygon::pointInPolygon(const Object& object) const {
    bool oddNodes = true;
    int first_col = -2;
    for (unsigned int i = 0; i < indices.size() - 2; ++i) {
        Eigen::Vector3f A(vertices[indices[i+0]], vertices[indices[i+0]+1], vertices[indices[i+0]+2]),
                        B(vertices[indices[i+1]], vertices[indices[i+1]+1], vertices[indices[i+1]+2]),
                        C(vertices[indices[i+2]], vertices[indices[i+2]+1], vertices[indices[i+2]+2]),
                        n;
        float d;
        calculateNormal(A, B, C, n, d);
        if (planeCrossed(n, d, object.getPosition(), object.getPassPosition(), object.getRadius()) &&
            polygonPoint(A, B, p)) {
            oddNodes = not oddNodes;
            if(first_col == -2) {
                first_col = i;
            }
        }
    }

    return oddNodes? first_col : -1;
}

bool Polygon::isColliding(Object &object) const
{
    if (physicsType == PhysicsType::Transparent) return false;
    if (sphereVsSphere(object.getPosition(), this->p, this->r2, object.getRadiusSqrt())) {
        int face = pointInPolygon(object);
        if (face > -1) {
            return false;
        }
    }
    return false;
}
