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

    std::vector<unsigned int> boxi = {0,1,2,3};
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
    Triangle *t = new Triangle(box, boxi, Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1);
    objects.first.push_back(t);

    Sphere *s = new Sphere({0.0f, 0.0f, 0.0f}, {0}, Object::ShaderType::Sphere, Eigen::Vector3f(0.2f,0.5f,1.0f),  Eigen::Vector3f(0.0f,0.0f,0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1);
    objects.first.push_back(s);


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for (int i = 1; i < 10; ++i) {
        float rx = dis(gen), rz = dis(gen), ry = dis(gen);
        Particle *b = new Particle(Eigen::Vector3f(0.0f, 0.2f*ry, 0.0f), Eigen::Vector3f(0.75f*(rx-0.5f), 0.5f, 0.75f*(rz-0.5f)), 0.05f);
        objects.second.push_back(b);
    }
    return true;
}
