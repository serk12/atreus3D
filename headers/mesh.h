#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>

#include "object.h"

class Mesh : public Object {
public:
    Mesh();
    Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u);
    Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices,const ShaderType programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const GLenum type);

    void render() const final;
    void event(QEvent *event) final;
    void forceUpdate() final;

private:
    GLenum type;

    bool collisionDetect(const std::list<Object*>& meshs) final;
    void propagateCollision(const std::list<Object*>& meshs) final;
};

#endif // MESH_H
