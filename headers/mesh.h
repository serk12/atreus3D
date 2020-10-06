#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>

#include "object.h"

class Mesh : public Object {
public:
    Mesh();
    Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices,const unsigned int programIndice, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const GLenum type);
    ~Mesh();

    void render() const final;
    void event(QEvent *event) final;

private:
    GLenum type = GL_TRIANGLES;

    void forceUpdate() final;
    void collisionDetect() final;
    bool possitionCorrect() final;
};

#endif // MESH_H
