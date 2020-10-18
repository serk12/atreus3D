#ifndef PARTICLE_H
#define PARTICLE_H
#include <GL/glew.h>
#include <list>

#include "object.h"
#include "utils.h"

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
    void forceUpdate() final;
    bool isColliding(Object& object) const final;
    float getRadius() const final;

    void addParticle(Particle* &p, const float distance);

    void setElasticityTerm(const float k_elas);
    void setDampingTerm(const float k_damp);
    void setDistancyTerm(const float d);

private:
    inline static const GLenum TYPE = GL_POINTS;
    inline static const float r = GENERAL_R;

    std::list<Particle*> links;
    std::list<float> linksDistance;
    float k_elas = GENERAL_K_ELAS, k_damp = GENERAL_K_DUMP;
    void collisionDetect(const std::list<Object*>& meshs) final;
    bool possitionCorrect() final;

};

#endif // PARTICLE_H
