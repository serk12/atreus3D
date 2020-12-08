#ifndef MESHTYPES_H
#define MESHTYPES_H

#include "mesh.h"
#include "utils.h"

class Sphere : public Mesh {
public:
    Sphere();
    Sphere(const std::vector<double> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m);
    Sphere(const std::vector<double> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u, const double r);

    double getRadius() const final;
    bool isColliding(Object& object) const final;

private:
    double r = GENERAL_RM;
    double r2 = r*r;
};


class Triangle : public Mesh {
public:
    Triangle();
    Triangle(const std::vector<double> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m);
    Triangle(const std::vector<double> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u,  const GLenum type);

    double getRadius() const final;
    bool isColliding(Object& object) const final;

private:
    Eigen::Vector3d n, A, B, C;
    double d;
    double area;
};


class Plane : public Mesh {
public:
    Plane();
    Plane(const std::vector<double> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m);
    Plane(const std::vector<double> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u, const GLenum type);

    double getRadius() const final;
    bool isColliding(Object& object) const final;

private:
    Eigen::Vector3d n;
    double d;
};


class Polygon : public Mesh {
public:
    Polygon();
    Polygon(const std::vector<double> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m);
    Polygon(const std::vector<double> vertices, const std::vector<unsigned int> indices, const ShaderType programIndice, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u, const GLenum type);

    double getRadius() const final;
    bool isColliding(Object& object) const final;

private:

};


#endif // MESHTYPES_H
