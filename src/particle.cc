#include <GL/glew.h>
#include <iostream>
#include "particle.h"

// https://imdoingitwrong.wordpress.com/tag/sph/

Particle::Particle() : Particle(ShaderType::Ball, Eigen::Vector3f(0.2f, 0.6f, 0.5f),
                                Eigen::Vector3f(0.0f, 0.0f, 0.0f),
                                Eigen::Vector3f(0.0002f, 0.0f, 0.0f), 0.05f, 0.96f, 0.670f) {}

Particle::Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u) : Particle(ShaderType::Ball, Eigen::Vector3f(0.6f, 0.2f, 0.5f), p, v, m, e, u) {}

Particle::Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const Eigen::Vector3f c) : Particle(ShaderType::Ball, c, p, v, m, e, u) {}

Particle::Particle(const ShaderType programIndex, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u)
{
    this->shaderType = programIndex;
    this->objectColor = color;
    this->p = p;
    this->v = v;
    this->m = m;
    this->e = e;
    this->u = u;
    this->_type = ObjectType::_Particle;

    //predefined
    vertices = {0.0f, 0.0f,  0.0f};
    indices = {0};
    qttyFaces = vertices.size();
    initSolver();
}

Particle::Particle(const ShaderType programIndex, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u,
                   const std::list<Particle*> links, const std::list<float> linksDistance) : Particle(programIndex, color, p, v, m, e, u)
{
    this->links = std::list<Particle*>(links);
    this->linksDistance = std::list<float>(linksDistance);
}
Particle::Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u,
                   const std::list<Particle*> links, const std::list<float> linksDistance) : Particle(ShaderType::Ball, Eigen::Vector3f(0.6f, 0.2f, 0.5f), p, v, m, e, u)
{
    this->links = std::list<Particle*>(links);
    this->linksDistance = std::list<float>(linksDistance);
}


void Particle::addParticle(Particle* &p, const float distance)
{
    links.push_back(p);
    linksDistance.push_back(distance);
}

float Particle::getRadius() const
{
    return r;
}


float Particle::getRadiusSqrt() const
{
    return r2;
}

Eigen::Matrix3f Particle::getInertiaMatrix() const
{
    return Eigen::Matrix3f::Identity() * (1.0f/5.0f* r2 * r2);
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
        std::list<float>::iterator itDis = linksDistance.begin();
        for (std::list<Particle*>::iterator it = links.begin(); it != links.end(); ++it, ++itDis) {
            Particle* l = *it;
            float dis = *itDis;
            Eigen::Vector3f diff  = p - l->p;
            Eigen::Vector3f diffV = v - l->v;
            Eigen::Vector3f direction = diff.normalized();
            f -= (k_elas * (diff.norm() - dis) + (k_damp * diffV.dot(direction))) * direction;
        }
    }
    else {
        f = Eigen::Vector3f::Zero();
    }

    tor = Eigen::Vector3f::Zero();
}

bool Particle::isColliding(Object &)
{
    return false;
}

void Particle::setDampingTerm(const float k_damp)
{
    this->k_damp = k_damp;
}

void Particle::setElasticityTerm(const float k_elas)
{
    this->k_elas = k_elas;
}

void Particle::setDistancyTerm(const float d)
{
    std::list<float>::iterator it = linksDistance.begin();
    float ref = *it;
    for (; it != linksDistance.end(); ++it) {
        if (*it != ref) return;
    }

    for (std::list<float>::iterator it = linksDistance.begin(); it != linksDistance.end(); ++it) {
        *it = d;
    }
}

void Particle::propagateCollision(const std::list<Object*>&) {}
