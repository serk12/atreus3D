#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>

#include "object.h"

class Mesh : public Object {
public:
    enum MeshType {Triangle = 0, Plane = 1, Sphere = 2,Polygon = 3};
    Mesh();
    Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const MeshType meshType);
    Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices,const unsigned int programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const GLenum type, const MeshType meshType);
    ~Mesh();

    void render() const final;
    void event(QEvent *event) final;

private:
    GLenum type = GL_TRIANGLES;
    MeshType meshType;

    void forceUpdate() final;
    void collisionDetect(const std::list<Object*>& meshs) final;
    bool possitionCorrect() final;
};

#endif // MESH_H
