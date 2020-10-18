#ifndef PARTICLE_H
#define PARTICLE_H
#include <GL/glew.h>
#include <list>

#include "object.h"

class Particle : public Object {
public:
    Particle();
    Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u);
    Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u,
             const std::list<Particle*> links, const std::list<float> linksDistance);

    Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const Eigen::Vector3f c);

    Particle(const ShaderType programIndex,const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u);
    Particle(const ShaderType programIndex, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u,
             const std::list<Particle*> links, const std::list<float> linksDistance);

    void render() const final;
    void event(QEvent *event) final;
    bool isColliding(Object& object) const final;
    float getRadius() const final;

    void addParticle(Particle* &p, const float distance);

    void setElasticityTerms(const float k_elas, const float k_damp);
    void setDistancyTerm(const float d);

private:
    inline static const GLenum TYPE = GL_POINTS;
    inline static const float r = 0.0175f;

    std::list<Particle*> links;
    std::list<float> linksDistance;
    float k_elas = 23.60f, k_damp = 2.30f;
    void forceUpdate() final;
    void collisionDetect(const std::list<Object*>& meshs) final;
    bool possitionCorrect() final;

};

#endif // PARTICLE_H
