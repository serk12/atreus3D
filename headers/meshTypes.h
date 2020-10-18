#ifndef MESHTYPES_H
#define MESHTYPES_H

#include "mesh.h"
#include "utils.h"

class Sphere : public Mesh {
public:
    Sphere();
    Sphere(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    Sphere(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const float r);

    float getRadius() const final;
    bool isColliding(Object& object) const final;

private:
    float r = GENERAL_RM;
    float r2 = r*r;
};


class Triangle : public Mesh {
public:
    Triangle();
    Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    Triangle(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u,  const GLenum type);

    float getRadius() const final;
    bool isColliding(Object& object) const final;

private:
    Eigen::Vector3f n, A, B, C;
    float d;
    float area;
};


class Plane : public Mesh {
public:
    Plane();
    Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    Plane(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const GLenum type);

    float getRadius() const final;
    bool isColliding(Object& object) const final;

private:
    Eigen::Vector3f n;
    float d;
};


class Polygon : public Mesh {
public:
    Polygon();
    Polygon(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    Polygon(const std::vector<float> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const GLenum type);

    float getRadius() const final;
    bool isColliding(Object& object) const final;

private:

};


#endif // MESHTYPES_H
