#ifndef MESH_H
#define MESH_H
#include <QOpenGLVertexArrayObject>
#include <vector>

#include "object.h"

class Mesh : public Object {
public:
    Mesh();
    Mesh(const char file[]);
    void render() final;
    void update(const float deltatime) final;
    void event(QEvent *event) final;
    bool load() final;
private:
    unsigned int VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int qttyFaces;
};

#endif // MESH_H
