#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>

#include "object.h"

class Mesh : public Object {
public:
    Mesh();
    Mesh(const std::vector<double> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u);
    Mesh(const std::vector<double> vertices, const std::vector<unsigned int> indices,const ShaderType programIndice, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u, const GLenum type);

    void render() const final;
    void event(QEvent *event) final;
    void forceUpdate() final;

private:
    GLenum type;

    bool collisionDetect(const std::list<Object*>& meshs) final;
    void propagateCollision(const std::list<Object*>& meshs) final;
};

#endif // MESH_H
