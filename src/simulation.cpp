#include "simulation.h"
#include "ui_mainwindow.h"

#include "particle.h"
#include "meshTypes.h"

#include <random>
#include <iostream>

Simulation::Simulation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Simulation)
{
    ui->setupUi(this);
}

Simulation::~Simulation()
{
    delete ui;
}


bool CubeScene(std::pair<std::list<Object*>, std::list<Object*> >& objects) {
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
    Plane *a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(0.0f, 1.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, 0.95f, 0.80f,  GL_LINE_LOOP);
    objects.first.push_back(a);

    box = {
            1.0f*rx,  1.0f*ry,  1.0f*rz, // 0
            1.0f*rx, -1.0f*ry,  1.0f*rz, // 2
           -1.0f*rx, -1.0f*ry,  1.0f*rz, // 6
           -1.0f*rx,  1.0f*ry,  1.0f*rz, // 4
        };
    a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(1.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, 0.95f, 0.80f, GL_LINE_STRIP);
    objects.first.push_back(a);

    box = {
       -1.0f*rx,  1.0f*ry, -1.0f*rz, // 5
       -1.0f*rx, -1.0f*ry, -1.0f*rz, // 7
        1.0f*rx, -1.0f*ry, -1.0f*rz, // 3
        1.0f*rx,  1.0f*ry, -1.0f*rz, // 1
        };
    a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(0.0f, 0.5f, 1.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, 0.95f, 0.80f, GL_LINE_STRIP);
    objects.first.push_back(a);

    box = {
           -1.0f*rx, -1.0f*ry,  1.0f*rz, // 6
           -1.0f*rx, -1.0f*ry, -1.0f*rz, // 7
           -1.0f*rx,  1.0f*ry, -1.0f*rz, // 5
           -1.0f*rx,  1.0f*ry,  1.0f*rz, // 4
        };
    a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(0.5f, 0.25f, 0.5f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, 0.95f, 0.80f, GL_LINE_STRIP);
    objects.first.push_back(a);

    box = {
            1.0f*rx,  1.0f*ry,  1.0f*rz, // 0
            1.0f*rx, -1.0f*ry,  1.0f*rz, // 2
            1.0f*rx, -1.0f*ry, -1.0f*rz, // 3
            1.0f*rx,  1.0f*ry, -1.0f*rz, // 1
        };
    a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(1.0f, 0.5f, 0.5f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, 0.95f, 0.80f, GL_LINE_STRIP);
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

    Sphere *s = new Sphere({0.0f, 0.0f, 0.0f}, {0}, Object::ShaderType::Sphere, Eigen::Vector3f(0.2f,0.5f,1.0f),  Eigen::Vector3f(0.0f,0.0f,0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1, 0.95f, 0.80f, 0.18f);
    objects.first.push_back(s);

    return true;
}

bool stringScene(std::pair<std::list<Object*>, std::list<Object*> >&objects)
{
    float rx = 0.75f;
    float ry = 0.3f;
    float rz = 0.75f;
    std::vector<float> box;
    std::vector<unsigned int> boxi;
    box = {
        -1.0f*rx, -1.0f*ry,  1.0f*rz, // 6
        1.0f*rx, -1.0f*ry,  1.0f*rz, // 2
        1.0f*rx, -1.0f*ry, -1.0f*rz, // 3
       -1.0f*rx, -1.0f*ry, -1.0f*rz, // 7
    };

    boxi = {0,1,2,3};
    Plane *a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(0.0f, 1.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, 0.95f, 0.80f,  GL_LINE_LOOP);
    objects.first.push_back(a);

    Particle* aux = new Particle(Eigen::Vector3f(0.0f, 1.0f, 0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1.0f, 0.95f, 3.80f);
    objects.second.push_back(aux);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    float r = dis(gen)-0.5f;
    float d = 0.01f;
    int qttyPar = 10;
    for (int i = 1; i < qttyPar; ++i) {
        Particle* c = new Particle(Eigen::Vector3f(0.0f - 0.2f*i, 0.0f, 0.0f),
                                   Eigen::Vector3f(0.0f*r, 2.0f*r, 0.0f*r),
                                   0.4f, 0.95f, 0.80f, Eigen::Vector3f(0.1f*i, 0.5f, 0.1f*i));
        objects.second.push_back(c);
        c->addParticle(aux, d);
        aux->addParticle(c, d);
        aux = c;
    }

    Particle* c = new Particle(Eigen::Vector3f(0.0f - 0.2*qttyPar, 0.0f, 0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), 1.0f, 0.95f, 0.80f);
    c->addParticle(aux, d);
    aux->addParticle(c, d);
    objects.second.push_back(c);
    return true;
}


bool Simulation::loadSim(std::pair<std::list<Object*>, std::list<Object*> >& objects, ScenaryType type)
{
    Simulation::scenaryType = type;
    switch (type) {
    case ScenaryType::String:
        return stringScene(objects);
    case ScenaryType::Cascade:
    case ScenaryType::Fountain:
    case ScenaryType::Debug:
    case ScenaryType::Rain:
    default:
        return CubeScene(objects);
    }
}

void Simulation::addParticle(std::list<Object*>& particleList)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    float rx = dis(gen)-0.5f, rz = dis(gen)-0.5f, ry = dis(gen)-0.5f;
    Particle *b = nullptr;
    switch (Simulation::scenaryType) {
    case ScenaryType::Cascade:
        b = new Particle(Eigen::Vector3f(0.0f+0.2f*rx, 0.3f+0.1f*ry, 0.0f+0.2f*rz), Eigen::Vector3f(2.0f*rx, 0.0f, 2.0f*rz), 0.05f, 0.95f, 0.80f);
        break;
    case ScenaryType::Fountain:
        b = new Particle(Eigen::Vector3f(0.0f+0.2f*rx, 0.5f+0.1f*ry, 0.0f+0.2f*rz), Eigen::Vector3f(2.0f*rx, 5.0f+5.0f*ry, 2.0f*rz), 0.05f, 0.95f, 0.80f);
        break;
    case ScenaryType::Rain:
        b = new Particle(Eigen::Vector3f((rx-0.5f)*0.2f, 0.85f, (rz-0.5f)*0.2f), Eigen::Vector3f(0.0f,0.0f,0.0f), 0.05f, 0.95f, 0.80f);
        break;
    case ScenaryType::Debug:
        b = new Particle(Eigen::Vector3f(0.0f, 0.85f, 0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), 0.05f, 0.95f, 0.80f);
        break;
    default:
        break;
    }
    if (b != nullptr) {
        b->load();
        particleList.push_back(b);
    }
}


void Simulation::on_GravityScale_valueChanged(int value)
{
    Object::setGravityScale(value/100.0f);
}


void Simulation::on_TopParticles_valueChanged(int value)
{
    Simulation::maxParticles = value;
}

void Simulation::on_BirdTime_valueChanged(int value)
{
    Simulation::birdTime = value;
}


void Simulation::on_LiveTime_valueChanged(int value)
{
    Simulation::liveTime = value;
}
