#include <GL/glew.h>
#include <iostream>
#include "particle.h"

Particle::Particle() : Particle(ShaderType::Sphere, Eigen::Vector3d(0.2f, 0.6f, 0.5f),
                                Eigen::Vector3d(0.0f, 0.0f, 0.0f),
                                Eigen::Vector3d(0.0002f, 0.0f, 0.0f), 0.05f, 0.96f, 0.670f) {}

Particle::Particle(const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u) : Particle(ShaderType::Sphere, Eigen::Vector3d(0.6f, 0.2f, 0.5f), p, v, m, e, u) {}

Particle::Particle(const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u, const Eigen::Vector3d c) : Particle(ShaderType::Sphere, c, p, v, m, e, u) {}

Particle::Particle(const ShaderType programIndex, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u)
{
    this->shaderType = programIndex;
    this->objectColor = color;
    this->p = p;
    this->v = v;
    this->m = m;
    this->e = e;
    this->u = u;

    //predefined
    vertices = {0.0f, 0.0f,  0.0f};
    indices = {0};
    qttyFaces = vertices.size();
    initSolver();
}

Particle::Particle(const ShaderType programIndex, const Eigen::Vector3d color, const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u,
                   const std::list<Particle*> links, const std::list<double> linksDistance) : Particle(programIndex, color, p, v, m, e, u)
{
    this->links = std::list<Particle*>(links);
    this->linksDistance = std::list<double>(linksDistance);
}
Particle::Particle(const Eigen::Vector3d p, const Eigen::Vector3d v, const double m, const double e, const double u,
                   const std::list<Particle*> links, const std::list<double> linksDistance) : Particle(ShaderType::Sphere, Eigen::Vector3d(0.6f, 0.2f, 0.5f), p, v, m, e, u)
{
    this->links = std::list<Particle*>(links);
    this->linksDistance = std::list<double>(linksDistance);
}


void Particle::addParticle(Particle* &p, const double distance)
{
    links.push_back(p);
    linksDistance.push_back(distance);
}

double Particle::getRadius() const
{
    return r;
}

void Particle::render() const
{
    renderType(Particle::TYPE);
}


void Particle::event(QEvent *) {}

void Particle::forceUpdate()
{
    if (physicsType == PhysicsType::Normal) {
        f = gravity*m;
        std::list<double>::iterator itDis = linksDistance.begin();
        for (std::list<Particle*>::iterator it = links.begin(); it != links.end(); ++it, ++itDis) {
            Particle* l = *it;
            double dis = *itDis;
            Eigen::Vector3d diff  = p - l->p;
            Eigen::Vector3d diffV = v - l->v;
            Eigen::Vector3d direction = diff.normalized();
            f -= (k_elas * (diff.norm() - dis) + (k_damp * diffV.dot(direction))) * direction;
        }
    }
    else {
        f = Eigen::Vector3d(0.0f, 0.0f, 0.0f);
    }

}

bool Particle::isColliding(Object &) const
{
    return false;
}

bool Particle::collisionDetect(const std::list<Object*>& meshs)
{
    bool result = false;
    for (Object* m : meshs) {
        if(m->isColliding(*this)) result = true;
    }
    return result;
}

void Particle::setDampingTerm(const double k_damp)
{
    this->k_damp = k_damp;
}

void Particle::setElasticityTerm(const double k_elas)
{
    this->k_elas = k_elas;
}

void Particle::setDistancyTerm(const double d)
{
    std::list<double>::iterator it = linksDistance.begin();
    double ref = *it;
    for (; it != linksDistance.end(); ++it) {
        if (*it != ref) return;
    }

    for (std::list<double>::iterator it = linksDistance.begin(); it != linksDistance.end(); ++it) {
        *it = d;
    }
}

void Particle::propagateCollision(const std::list<Object*>&)
{
//    for (std::list<Particle*>::iterator it = links.begin(); it != links.end(); ++it) {
//        // first do the correction then check if errors
//        bool result = (*it)->collisionDetect(meshs);
//        if (result) (*it)->propagateCollision(meshs);
//    }
}
