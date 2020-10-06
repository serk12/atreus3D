#ifndef PARTICLE_H
#define PARTICLE_H
#include <GL/glew.h>

#include "object.h"

class Particle : public Object {
public:
    Particle();
    Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    Particle(const unsigned int programIndex,const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m);
    ~Particle();

    void render() const final;
    void event(QEvent *event) final;

private:
    inline static const GLenum TYPE = GL_POINTS;

    void forceUpdate() final;
    void collisionDetect() final;
    bool possitionCorrect() final;
};

#endif // PARTICLE_H
