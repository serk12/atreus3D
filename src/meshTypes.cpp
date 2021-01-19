#include "meshTypes.h"

#include <iostream>

// ******* //
// GENERAL //
// ******* //

const float __TIME_SCALAR = 0.125f;

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


void calculateNormal(const Eigen::Vector3f& c, const Eigen::Vector3f& P, Eigen::Vector3f& n, float& d)
{
    n = (P - c).normalized();
    d = -(n.x()*P.x() + n.y()*P.y() + n.z()*P.z());

    if (d <= 0.0f) {
        d = -d;
        n = -1.0f*n;
    }
}

inline bool planeCrossed(const Eigen::Vector3f& n, const float d, const Eigen::Vector3f& p, const Eigen::Vector3f& p_pass, const Eigen::Vector3f& v, const float r = 0.0f)
{
    float d_aux = d - (r*n).norm();
    return (n.dot(p - v*__TIME_SCALAR) + d_aux) * (n.dot(p_pass) + d_aux) <= 0.0f;
}

inline bool sphereVsSphere(Eigen::Vector3f p1, Eigen::Vector3f p2, float rs1, float rs2)
{
    Eigen::Vector3f diff_p = p1 - p2;
    return (((diff_p.transpose() * diff_p) - (rs1 + rs2)) <= 0.0f);
}


inline float areaTrangle(const Eigen::Vector3f& A, const Eigen::Vector3f& B, const Eigen::Vector3f& C)
{
    Eigen::Vector3f BA = (B - A);
    Eigen::Vector3f CA = (C - A);

    return (BA.cross(CA)).norm()*0.5f;
}

inline int triangleCrossed(const Eigen::Vector3f& n, const float d, const float r, const Eigen::Vector3f& p, const Eigen::Vector3f& p_pass, const Eigen::Vector3f& v,const Eigen::Vector3f& A, const Eigen::Vector3f& B, const Eigen::Vector3f& C, const float area)
{
    int result = 0;
    if (planeCrossed(-n, -d, p, p_pass, v, r)) {
        result = -1;
    }
    else if (planeCrossed(n, d, p, p_pass, v, r)) {
        result = 1;
    }
    if ((result != 0) and ((areaTrangle(p, B, C) + areaTrangle(A, p, C) + areaTrangle(A, B, p) - area) <= r)) {
        return result;
    }
    return 0;
}

bool resolveSphereVsSphere(Object& a, Object& b)
{
    Eigen::Vector3f p_n = b.getPosition() + b.getVelocity()*__TIME_SCALAR;
    Eigen::Vector3f p_a = b.getPassPosition();
    if (sphereVsSphere(p_n, a.getPosition(), a.getRadiusSqrt(), b.getRadiusSqrt())) {
        Eigen::Vector3f c = a.getPosition();
        Eigen::Vector3f v = b.getVelocity();
        float alpha = v.dot(v), beta = (2.0f*v).dot(p_a-c), gamma = c.dot(c) + p_a.dot(p_a)-(2.0f*p_a).dot(c) - a.getRadiusSqrt();
        float aux_1 = sqrt(beta*beta-4.0f*alpha*gamma), aux_2 = 2.0f*alpha;
        float dir1 = (-beta+aux_1)/(aux_2);
        float dir2 = (-beta-aux_1)/(aux_2);
        if (std::isnan(dir1) && std::isnan(dir2)) return false;
        Eigen::Vector3f P = b.getPosition() + ((dir1 >= 0.0f)? dir2 : dir1)*v;
        Eigen::Vector3f n;
        float d;
        calculateNormal(c, P, n, d);
        b.correctObject(n, d, Object::POSITIVE_FORCE, Object::POSITIVE_FORCE);
        a.correctObject(n, d, Object::POSITIVE_FORCE, Object::NEGATIVE_FORCE);
    }
    return false;
}

bool resolveTriangleVsSphere(Triangle& a, Object& b)
{
    Eigen::Vector3f aux_A = a.getVertex(0) + a.getPosition();
    Eigen::Vector3f aux_B = a.getVertex(1) + a.getPosition();
    Eigen::Vector3f aux_C = a.getVertex(2) + a.getPosition();
    Eigen::Vector3f n = a.getN();
    float d = a.getD();
    int collision = triangleCrossed(n, d, b.getRadius(), b.getPosition(), b.getPassPosition(), b.getVelocity(), aux_A, aux_B, aux_C, a.getArea());
    if (collision != 0) {
        b.correctObject(n, d, Object::POSITIVE_OFFSET, Object::POSITIVE_FORCE);
        a.correctObject(n, d, Object::NEGATIVE_OFFSET, Object::NEGATIVE_FORCE);
        return true;
    }
    return false;
}

bool resolvePlaneVsSphere(Plane& a, Object& b)
{
    Eigen::Vector3f n = a.getN();
    float d = a.getD();
    if(planeCrossed(n, d, b.getPosition(), b.getPassPosition(), b.getVelocity(), b.getRadius())) {
        b.correctObject(n, d, Object::NEGATIVE_OFFSET, Object::POSITIVE_FORCE);
        a.correctObject(n, d, Object::NEGATIVE_OFFSET, Object::NEGATIVE_FORCE);
        return true;
    }
    return false;
}

// ****** //
// SPHERE //
// ****** //

Sphere::Sphere()
    : Sphere(Eigen::Vector3f(0.5f, 1.0f, 0.5f), Eigen::Vector3f::Zero(), Eigen::Vector3f::Zero(), -1, 0.95f, 0.80f, 0.5f) {}

Sphere::Sphere(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float r)
    : Sphere(Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, 0.95, 0.30f, r) {}

Sphere::Sphere(const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const float r)
    : Mesh({0.0f, 0.0f, 0.0f}, {0}, ShaderType::Ball, color, p, v, m, e, u, GL_POINTS)
{
    this->r = r;
    this->r2 = r*r;
    this->_type = Object::ObjectType::_Sphere;
    //predefined
    initSolver();
}

float Sphere::getRadius() const
{
    return r;
}

float Sphere::getRadiusSqrt() const
{
    return this->r2;
}


bool Sphere::isColliding(Object &object)
{
    if (physicsType == PhysicsType::Transparent) return false;
    if (object.getType() == ObjectType::_Particle or
       (object.getType() == ObjectType::_Sphere and object.getID() < this->getID())) {
        return resolveSphereVsSphere((*this), object);
    }
    else if (object.getID() < this->getID()){
        switch (object.getType()) {
        case ObjectType::_Plane:
                return resolvePlaneVsSphere(static_cast<Plane&>(object), *this);
        case ObjectType::_Polygon:
                break;
        case ObjectType::_Triangle:
                return resolveTriangleVsSphere(static_cast<Triangle&>(object), *this);
        default:
            std::cout << "collision problem" << std::endl;
        break;
        }
    }
    return false;
}


Eigen::Matrix3f Sphere::getInertiaMatrix() const
{
    return Eigen::Matrix3f::Identity() * (1.0f/5.0f* r2 + r2);
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
    //predefined
    initSolver();
}


float Triangle::getRadius() const
{
    return r;
}

float Triangle::getRadiusSqrt() const
{
    return r2;
}

Eigen::Vector3f Triangle::getVertex(int id) const
{
    if (id%3 == 0) return A;
    if (id%3 == 1) return B;
    if (id%3 == 2) return C;
    return Eigen::Vector3f::Zero();
}

float Triangle::getD()
{
    calculateNormal(A + p, B + p, C + p, n, d);
    return d;
}
float Triangle::getArea() const {return area;}
Eigen::Vector3f Triangle::getN() const {return n;}

bool Triangle::isColliding(Object &object)
{
    if (physicsType == PhysicsType::Transparent) return false;
    if (object.getType() == ObjectType::_Particle or object.getType() == ObjectType::_Sphere) {
        return resolveTriangleVsSphere(*this, object);
    }
    else if (object.getID() < this->getID()){
        switch (object.getType()) {
        case ObjectType::_Plane:
                break;
        case ObjectType::_Polygon:
                break;
        case ObjectType::_Triangle:
                break;
        default:
            std::cout << "collision problem" << std::endl;
        break;
        }
    }
    return false;
}


Eigen::Matrix3f Triangle::getInertiaMatrix() const
{
    float a = (getVertex(0) - getVertex(1)).norm();
    float b = 2 * getArea() / a;
    float c2 = 0.0001f;
    Eigen::Matrix3f result = Eigen::Matrix3f::Identity();
    result(0,0) *= (b*b + c2);
    result(1,1) *= (a*a + c2);
    result(2,2) *= (a*a + b*b);
    return result;
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
    //predefined
    initSolver();
}


float Plane::getRadius() const
{
    return std::numeric_limits<float>::infinity();
}

float Plane::getRadiusSqrt() const
{
    return std::numeric_limits<float>::infinity();
}

float Plane::getD() const {return d;}
Eigen::Vector3f Plane::getN() const {return n;}

bool Plane::isColliding(Object &object)
{
    if (physicsType == PhysicsType::Transparent) return false;
    if (object.getType() == ObjectType::_Particle or object.getType() == ObjectType::_Sphere) {
        return resolvePlaneVsSphere(*this, object);
    }
    else if (object.getID() < this->getID()){
        switch (object.getType()) {
        case ObjectType::_Plane:
                break;
        case ObjectType::_Polygon:
                break;
        case ObjectType::_Triangle:
                break;
        default:
            std::cout << "collision problem" << std::endl;
        break;
        }
    }
    return false;
}

Eigen::Matrix3f Plane::getInertiaMatrix() const
{
    // technically this is infinite....
    return Eigen::Matrix3f::Identity() * (1000.0f);
}

// ******* //
// POLYGON //
// ******* //

Polygon::Polygon() : Mesh() {}

Polygon::Polygon(const std::string& path, const Eigen::Vector3f offSet, const float scale, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
    : Polygon(path, offSet, scale, ShaderType::Vanilla, Eigen::Vector3f(0.5f, 1.0f, 0.5f), p, v, m, 0.95f, 0.250f) {}

Polygon::Polygon(const std::string& path, const Eigen::Vector3f offSet, const float scale, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u)
    : Mesh({}, {}, programIndice, color, p, v, m, e, u, loadModel(path, offSet, scale)) {
    //predefined
    initSolver();
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
        if (planeCrossed(n, d, object.getPosition(), object.getPassPosition(), object.getVelocity(), object.getRadius()) &&
            polygonPoint(A, B, p)) {
            oddNodes = not oddNodes;
            if(first_col == -2) {
                first_col = i;
            }
        }
    }

    return oddNodes? first_col : -1;
}

bool Polygon::isColliding(Object &object)
{
    if (physicsType == PhysicsType::Transparent) return false;
    if (object.getType() == ObjectType::_Particle) {
        if (sphereVsSphere(object.getPosition(), this->p, this->r2, object.getRadiusSqrt())) {
            int face = pointInPolygon(object);
            if (face > -1) {
                return false;
            }
        }
    }
    else if (object.getID() < this->getID()){
        return false;
    }
    return false;
}


Eigen::Matrix3f Polygon::getInertiaMatrix() const
{
    return Eigen::Matrix3f::Identity() * (1.0f/3.0f* r2 + r2);
}
