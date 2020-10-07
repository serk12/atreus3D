#ifndef PARTICLE_H
#define PARTICLE_H
#include <GL/glew.h>
#include <list>

#include "object.h"

class Particle : public Object {
public:
    Particle();
    Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    Particle(const ShaderType programIndex,const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);

    void render() const final;
    void event(QEvent *event) final;
    bool isColliding(Eigen::Vector3f& p, Eigen::Vector3f& p_pass, Eigen::Vector3f& v, const float r) const final;

private:
    inline static const GLenum TYPE = GL_POINTS;
    inline static const float r = 0.025f;

    void forceUpdate() final;
    void collisionDetect(const std::list<Object*>& meshs) final;
    bool possitionCorrect() final;

    float getRadius() const final;
};

#endif // PARTICLE_H
