#include <GL/glew.h>
#include <iostream>
#include "particle.h"

Particle::Particle() : Particle(1, Eigen::Vector3f(0.2f, 0.6f, 0.5f),
                                Eigen::Vector3f(0.0f, 0.0f, 0.0f),
                                Eigen::Vector3f(0.0f, 0.0f, 0.0f), 0.05f) {}

Particle::Particle(const unsigned int programIndex,const Eigen::Vector3f color, const Eigen::Vector3f p, const Eigen::Vector3f v, const float m)
{
    this->program = Object::programsList[programIndex]; //vanilla
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


void Particle::render() const
{
    renderType(Particle::TYPE);
}

void Particle::event(QEvent *) {}

void Particle::forceUpdate() {}
void Particle::collisionDetect() {}
bool Particle::possitionCorrect() {return false;}