#include <GL/glew.h>
#include <iostream>
#include "particle.h"

Particle::Particle() : Particle(ShaderType::Sphere, Eigen::Vector3f(0.2f, 0.6f, 0.5f),
                                Eigen::Vector3f(0.0f, 0.0f, 0.0f),
                                Eigen::Vector3f(0.0002f, 0.0f, 0.0f), 0.05f, 0.96f, 0.670f) {}

Particle::Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u) : Particle(ShaderType::Sphere, Eigen::Vector3f(0.6f, 0.2f, 0.5f), p, v, m, e, u) {}

Particle::Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u, const Eigen::Vector3f c) : Particle(ShaderType::Sphere, c, p, v, m, e, u) {}

Particle::Particle(const ShaderType programIndex, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u)
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

Particle::Particle(const ShaderType programIndex, const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u,
                   const std::list<Particle*> links, const std::list<float> linksDistance) : Particle(programIndex, color, p, v, m, e, u)
{
    this->links = std::list<Particle*>(links);
    this->linksDistance = std::list<float>(linksDistance);
}
Particle::Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m, const float e, const float u,
                   const std::list<Particle*> links, const std::list<float> linksDistance) : Particle(ShaderType::Sphere, Eigen::Vector3f(0.6f, 0.2f, 0.5f), p, v, m, e, u)
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
            Eigen::Vector3f diff = p - l->p;
            Eigen::Vector3f diffV = v - l->v;
            Eigen::Vector3f direction = diff.normalized();
            f -= (k_e * (diff.norm() - dis) + (k_d * diffV.dot(direction))) * direction;
        }
    }
    else {
        f = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    }

}
// (yo: b) : a , c

bool Particle::isColliding(Object &) const
{
    return false;
}

void Particle::collisionDetect(const std::list<Object*>& meshs)
{
    for (Object* m : meshs) {
        m->isColliding(*this);
    }
}

bool Particle::possitionCorrect() {return false;}
