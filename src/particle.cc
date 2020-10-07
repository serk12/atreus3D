#include <GL/glew.h>
#include <iostream>
#include "particle.h"

Particle::Particle() : Particle(ShaderType::Sphere, Eigen::Vector3f(0.2f, 0.6f, 0.5f),
                                Eigen::Vector3f(0.0f, 0.0f, 0.0f),
                                Eigen::Vector3f(0.0002f, 0.0f, 0.0f), 0.05f) {}

Particle::Particle(const Eigen::Vector3f p, const Eigen::Vector3f v, const float m) : Particle(ShaderType::Sphere, Eigen::Vector3f(0.6f, 0.2f, 0.5f), p, v, m) {}

Particle::Particle(const ShaderType programIndex,const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
{
    this->shaderType = programIndex;
    this->objectColor = color;
    this->p = p;
    this->v = v;
    this->m = m;

    //predefined
    vertices = {0.0f, 0.0f,  0.0f};
    indices = {0};
    qttyFaces = vertices.size();
    initSolver();
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
    this->f = gravity*m;
}

void Particle::isColliding(Eigen::Vector3f&, Eigen::Vector3f&, Eigen::Vector3f&, const float) const
{

}

void Particle::collisionDetect(const std::list<Object*>& meshs)
{
    for (Object* m : meshs){
        m->isColliding(p, p_pass, v, r);
    }
}

bool Particle::possitionCorrect() {return false;}
