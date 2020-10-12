#include "simulation.h"

#include "particle.h"
#include "meshTypes.h"

#include <random>
#include <iostream>

bool Simulation::loadSim(std::pair<std::list<Object*>, std::list<Object*> >& objects)
{
    float rx = 0.75f;
    float ry = 0.3f;
    float rz = 0.75f;
    std::vector<float> box;
    std::vector<unsigned int> boxi;
//    std::vector<float> box = {
//        1.0f*rx,  1.0f*ry,  1.0f*rz, // 0
//        1.0f*rx,  1.0f*ry, -1.0f*rz, // 1
//        1.0f*rx, -1.0f*ry,  1.0f*rz, // 2
//        1.0f*rx, -1.0f*ry, -1.0f*rz, // 3
//                                     //
//       -1.0f*rx,  1.0f*ry,  1.0f*rz, // 4
//       -1.0f*rx,  1.0f*ry, -1.0f*rz, // 5
//       -1.0f*rx, -1.0f*ry,  1.0f*rz, // 6
//       -1.0f*rx, -1.0f*ry, -1.0f*rz, // 7
//    };

    box = {
        -1.0f*rx, -1.0f*ry,  1.0f*rz, // 6
        1.0f*rx, -1.0f*ry,  1.0f*rz, // 2
        1.0f*rx, -1.0f*ry, -1.0f*rz, // 3
       -1.0f*rx, -1.0f*ry, -1.0f*rz, // 7
        };

    boxi = {0,1,2,3};
    Plane *a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(0.0f, 1.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, GL_LINE_LOOP);
    objects.first.push_back(a);

    box = {
            1.0f*rx,  1.0f*ry,  1.0f*rz, // 0
            1.0f*rx, -1.0f*ry,  1.0f*rz, // 2
           -1.0f*rx, -1.0f*ry,  1.0f*rz, // 6
           -1.0f*rx,  1.0f*ry,  1.0f*rz, // 4
        };
    a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(1.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, GL_LINE_STRIP);
    objects.first.push_back(a);

    box = {
       -1.0f*rx,  1.0f*ry, -1.0f*rz, // 5
       -1.0f*rx, -1.0f*ry, -1.0f*rz, // 7
        1.0f*rx, -1.0f*ry, -1.0f*rz, // 3
        1.0f*rx,  1.0f*ry, -1.0f*rz, // 1
        };
    a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(0.0f, 0.5f, 1.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, GL_LINE_STRIP);
    objects.first.push_back(a);

    box = {
           -1.0f*rx, -1.0f*ry,  1.0f*rz, // 6
           -1.0f*rx, -1.0f*ry, -1.0f*rz, // 7
           -1.0f*rx,  1.0f*ry, -1.0f*rz, // 5
           -1.0f*rx,  1.0f*ry,  1.0f*rz, // 4
        };
    a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(0.5f, 0.25f, 0.5f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, GL_LINE_STRIP);
    objects.first.push_back(a);

    box = {
            1.0f*rx,  1.0f*ry,  1.0f*rz, // 0
            1.0f*rx, -1.0f*ry,  1.0f*rz, // 2
            1.0f*rx, -1.0f*ry, -1.0f*rz, // 3
            1.0f*rx,  1.0f*ry, -1.0f*rz, // 1
        };
    a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(1.0f, 0.5f, 0.5f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, GL_LINE_STRIP);
    objects.first.push_back(a);


    float r = 1.45f;
    box = {
        0.0f*r, 0.5f*r, 0.0f*r,
        1.0f*r, 0.0f*r, 0.0f*r,
        0.0f*r, 0.0f*r, 1.0f*r,
    };
    boxi = {
        0,1,2,
        2,1,0,
    };
    Triangle *t = new Triangle(box, boxi, Eigen::Vector3f(-0.75f, -0.50f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1);
    objects.first.push_back(t);

    Sphere *s = new Sphere({0.0f, 0.0f, 0.0f}, {0}, Object::ShaderType::Sphere, Eigen::Vector3f(0.2f,0.5f,1.0f),  Eigen::Vector3f(0.0f,0.0f,0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1);
    objects.first.push_back(s);

    for (int i = 0; i < 0; ++i) {
        Simulation::addParticle(objects.second);
    }
    return true;
}

void Simulation::addParticle(std::list<Object*>& particleList)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    float rx = dis(gen)-0.5f, rz = dis(gen)-0.5f, ry = dis(gen)-0.5f;
    Particle *b = new Particle(Eigen::Vector3f(0.0f+0.2f*rx, 0.3f+0.1f*ry, 0.0f), Eigen::Vector3f(0.1f*rx, 0.5f, 0.1f*rz), 0.05f);
    //Particle *b = new Particle(Eigen::Vector3f((rx-0.5f)*0.2f, 0.85f, (rz-0.5f)*0.2f), Eigen::Vector3f(0.0f,0.0f,0.0f), 0.05f);
    //Particle *b = new Particle(Eigen::Vector3f(0.0f, 0.85f, 0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), 0.05f);
    b->load();
    particleList.push_back(b);
}
