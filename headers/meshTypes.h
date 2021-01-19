#ifndef MESHTYPES_H
#define MESHTYPES_H

#include "mesh.h"
#include "utils.h"
#include "model.h"
#include "particle.h"
#include <iostream>

class Sphere : public Mesh {
public:
    virtual ~Sphere() {}
    Sphere();
    Sphere(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float r);
    Sphere(const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const float r);

    float getRadius() const final;
    float getRadiusSqrt() const final;
    bool isColliding(Object& object) final;
    Eigen::Matrix3f getInertiaMatrix() const final;

private:
    float r = GENERAL_RM;
    float r2 = r*r;
};


class Triangle : public Mesh {
public:
    Triangle();
    virtual ~Triangle() {}
    Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u,  const GLenum type);

    float getRadius() const final;
    float getRadiusSqrt() const final;
    bool isColliding(Object& object) final;
    Eigen::Matrix3f getInertiaMatrix() const final;
    Eigen::Vector3f getVertex(int id) const;
    float getD();
    float getArea() const;
    Eigen::Vector3f getN() const;
    float r;
    float r2;

private:
    Eigen::Vector3f n, A, B, C;
    float d;
    float area;
};


class Plane : public Mesh {
public:
    Plane();
    virtual ~Plane() {}
    Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const GLenum type);
    float getD() const;
    Eigen::Vector3f getN() const;

    float getRadius() const final;
    float getRadiusSqrt() const final;
    bool isColliding(Object& object) final;
    Eigen::Matrix3f getInertiaMatrix() const final;

private:
    Eigen::Vector3f n;
    float d;
};


class Polygon : public Mesh {
public:
    Polygon();
    virtual ~Polygon() {}
    Polygon(const std::string& path, const Eigen::Vector3f offSet, const float scale, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    Polygon(const std::string& path, const Eigen::Vector3f offSet, const float scale, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u);

    float getRadius() const final;
    float getRadiusSqrt() const final;
    Eigen::Matrix3f getInertiaMatrix() const final;
    bool isColliding(Object& object) final;
    GLenum loadModel(const std::string& path, const Eigen::Vector3f offSet, const float scale);
    int pointInPolygon(const Object &object) const;
    Model model;
    float maxX, maxY, maxZ, minY, minX, minZ, r, r2;

private:

};


#endif // MESHTYPES_H
