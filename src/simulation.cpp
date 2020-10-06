#include "simulation.h"

#include "particle.h"
#include "mesh.h"

#include <random>
#include <iostream>

bool Simulation::loadSim(std::pair<std::list<Object*>, std::list<Object*> >& objects)
{
    float rx = 0.75f;
    float ry = 0.3f;
    float rz = 0.75f;
    std::vector<float> box = {
         1.0f*rx,  1.0f*ry,  1.0f*rz,
         1.0f*rx,  1.0f*ry, -1.0f*rz,
         1.0f*rx, -1.0f*ry,  1.0f*rz,
         1.0f*rx, -1.0f*ry, -1.0f*rz,

        -1.0f*rx,  1.0f*ry,  1.0f*rz,
        -1.0f*rx,  1.0f*ry, -1.0f*rz,
        -1.0f*rx, -1.0f*ry,  1.0f*rz,
        -1.0f*rx, -1.0f*ry, -1.0f*rz,
    };
    std::vector<unsigned int> boxi = {
        0,2,3,1,
        0,4,
        6,7,5,4,
        6,7,
        5,1,3,7,
        5,4,
        0,2,6,4,
        0,2,
        6,7,3,2,
        0
    };

    Mesh *a = new Mesh(box, boxi, 0, Eigen::Vector3f(1.0f, 0.5f, 0.5f), Eigen::Vector3f(0.0f, 0.0f, 0.0f),Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, GL_LINE_STRIP, Mesh::MeshType::Plane);
    objects.first.push_back(a);

    float r = 0.45f;
    box = {
        0.0f*r, 0.5f*r, 0.0f*r,
        1.0f*r, 0.0f*r, 0.0f*r,
        0.0f*r, 0.0f*r, 1.0f*r,
    };
    boxi = {
        0,1,2,
        2,1,0,
    };
    a = new Mesh(box, boxi, Eigen::Vector3f(-0.25f, -0.25f, -0.25f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, Mesh::MeshType::Triangle);
    objects.first.push_back(a);

    a = new Mesh({0.0f, 0.0f, 0.0f}, {0}, Object::ShaderType::Sphere, Eigen::Vector3f(0.2f,0.5f,1.0f),  Eigen::Vector3f(0.0f,0.0f,0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1, GL_POINTS, Mesh::MeshType::Sphere);
    objects.first.push_back(a);


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for (int i = 1; i < 50; ++i) {
        float rx = dis(gen), rz = dis(gen), ry = dis(gen);
        Particle *b = new Particle(Eigen::Vector3f(0.0f, 0.2f*ry, 0.0f), Eigen::Vector3f(0.00004f*(rx-0.5f), 0.00005f, 0.00004f*(rz-0.5f)), 0.05f);
        objects.second.push_back(b);
    }
    return true;
}
