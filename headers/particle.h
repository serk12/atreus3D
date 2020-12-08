#ifndef PARTICLE_H
#define PARTICLE_H
#include <GL/glew.h>
#include <list>

#include "object.h"
#include "utils.h"

class Particle : public Object {
public:
    Particle();
    Particle(const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u);
    Particle(const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u,
             const std::list<Particle*> links, const std::list<double> linksDistance);

    Particle(const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u, const Eigen::Vector3d c);

    Particle(const ShaderType programIndex,const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u);
    Particle(const ShaderType programIndex, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u,
             const std::list<Particle*> links, const std::list<double> linksDistance);

    void render() const final;
    void event(QEvent *event) final;
    bool isColliding(Object& object) const final;
    double getRadius() const final;
    void forceUpdate() final;

    void addParticle(Particle* &p, const double distance);
    void setElasticityTerm(const double k_elas);
    void setDampingTerm(const double k_damp);
    void setDistancyTerm(const double d);

private:
    inline static const GLenum TYPE = GL_POINTS;
    inline static const double r = GENERAL_R;

    std::list<Particle*> links;
    std::list<double> linksDistance;
    double k_elas = GENERAL_K_ELAS, k_damp = GENERAL_K_DUMP;
    bool collisionDetect(const std::list<Object*>& meshs) final;
    void propagateCollision(const std::list<Object*>& meshs) final;

};

#endif // PARTICLE_H
