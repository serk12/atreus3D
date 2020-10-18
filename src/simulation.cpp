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
    ui->SolverMethod->insertItem(ui->SolverMethod->count(), "Euler", Object::SolverType::Euler);
    ui->SolverMethod->insertItem(ui->SolverMethod->count(), "SemiEuler", Object::SolverType::SemiEuler);
    ui->SolverMethod->insertItem(ui->SolverMethod->count(), "Verlet", Object::SolverType::Verlet);

    ui->ScenaryType->insertItem(ui->ScenaryType->count(), "Cascade", ScenaryType::Cascade);
    ui->ScenaryType->insertItem(ui->ScenaryType->count(), "Rain", ScenaryType::Rain);
    ui->ScenaryType->insertItem(ui->ScenaryType->count(), "Fountain", ScenaryType::Fountain);
    ui->ScenaryType->insertItem(ui->ScenaryType->count(), "String", ScenaryType::String);
    ui->ScenaryType->insertItem(ui->ScenaryType->count(), "Debug", ScenaryType::Debug);
    ui->ScenaryType->insertItem(ui->ScenaryType->count(), "Debug String", ScenaryType::DebugS);

    ui->BirdTime->setValue(Simulation::birdTime);
    ui->Kd->setValue(Simulation::k_d*100);
    ui->LiveTime->setValue(Simulation::liveTime);
    ui->TopParticles->setValue(Simulation::maxParticles);
    ui->GravityScale->setValue(Simulation::gravityScale*100);
    ui->SolverMethod->setCurrentIndex(int(Simulation::solverType));
    ui->ScenaryType->setCurrentIndex(int(Simulation::scenaryType));
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

    Sphere *s = new Sphere({0.0f, 0.0f, 0.0f}, {0}, Object::ShaderType::Sphere, Eigen::Vector3f(0.2f,0.5f,1.0f),  Eigen::Vector3f(0.0f,0.0f,0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1, 0.95f, 0.80f, 0.18f);
    objects.first.push_back(s);

    Particle* aux = new Particle(Eigen::Vector3f(1.0f, 1.0f, 0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1.0f, 0.95f, 0.80f);
    objects.second.push_back(aux);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    float r = dis(gen)-0.5f;
    float d = 0.01f;
    int qttyPar = 10;
    for (int i = 1; i < qttyPar; ++i) {
        Particle* c = new Particle(Eigen::Vector3f(1.0f - 0.2f*i, 1.0f, 0.0f),
                                   Eigen::Vector3f(0.0f*r, 2.0f*r, 0.0f*r),
                                   0.4f, 0.95f, 0.80f, Eigen::Vector3f(0.1f*i, 0.5f, 0.1f*i));
        objects.second.push_back(c);
        c->addParticle(aux, d);
        aux->addParticle(c, d);
        aux = c;
    }

    Particle* c = new Particle(Eigen::Vector3f(1.0f - 0.2*qttyPar, 1.0f, 0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), 1.0f, 0.95f, 0.80f);
    c->addParticle(aux, d);
    aux->addParticle(c, d);
    objects.second.push_back(c);
    return true;
}


bool Simulation::loadSim(std::pair<std::list<Object*>, std::list<Object*> >& objects)
{
    Object::setSolverModel(Simulation::solverType);
    Object::setKd(Simulation::k_d);
    Object::setGravityScale(Simulation::gravityScale);
    bool result;
    switch (Simulation::scenaryType) {
    case ScenaryType::DebugS:
    case ScenaryType::String:
        Simulation::liveTime = 1000000000;
        result = stringScene(objects);
        break;
    case ScenaryType::Cascade:
    case ScenaryType::Fountain:
    case ScenaryType::Debug:
    case ScenaryType::Rain:
    default:
        result = CubeScene(objects);
        break;
    }
    return result;
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
        b = new Particle(Eigen::Vector3f(0.0f+0.2f*rx, 0.3f+0.1f*ry, 0.0f+0.2f*rz), Eigen::Vector3f(2.0f*rx, 0.0f, 2.0f*rz), m, e, u);
        break;
    case ScenaryType::Fountain:
        b = new Particle(Eigen::Vector3f(0.0f+0.2f*rx, 0.5f+0.1f*ry, 0.0f+0.2f*rz), Eigen::Vector3f(2.0f*rx, 5.0f+5.0f*ry, 2.0f*rz), m, e, u);
        break;
    case ScenaryType::Rain:
        b = new Particle(Eigen::Vector3f((rx-0.5f)*0.2f, 0.85f, (rz-0.5f)*0.2f), Eigen::Vector3f(0.0f,0.0f,0.0f), m, e, u);
        break;
    case ScenaryType::Debug:
        b = new Particle(Eigen::Vector3f(0.0f, 0.85f, 0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), m, e, u);
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
    std::string text = "Gravity Scale: " + std::to_string(value) + "%";
    ui->GravityScaleLab->setText(QString::fromUtf8(text.c_str()));
    Simulation::gravityScale = value/100.0f;
    Object::setGravityScale(Simulation::gravityScale);
}

std::string int2string(int value, int n = 3)
{
    std::ostringstream out;
    out.precision(n);
    float div = 10.0f;
    for (int i = 1; i < n; ++i){
        div *= 10.0f;
    }
    out << std::fixed << value/div;
    return out.str();
}


void Simulation::on_TopParticles_valueChanged(int value)
{
    std::string text = "Birth Time: #" + std::to_string(value);
    ui->MaxParticleLab->setText(QString::fromUtf8(text.c_str()));
    Simulation::maxParticles = value;
}

void Simulation::on_BirdTime_valueChanged(int value)
{
    std::string text = "Birth Time: " + int2string(value) + "s";
    ui->BirthTimeLab->setText(QString::fromUtf8(text.c_str()));
    Simulation::birdTime = value;
    ui->openGLWidget->setBirthTime(Simulation::birdTime);
}


void Simulation::on_LiveTime_valueChanged(int value)
{
    std::string text = "Live Time: " + int2string(value) + "s";
    ui->LiveTimeLab->setText(QString::fromUtf8(text.c_str()));
    Simulation::liveTime = value;
}

void Simulation::on_Kd_valueChanged(int value)
{
    std::string text = "k_d: " + int2string(value, 2);
    ui->KdLab->setText(QString::fromUtf8(text.c_str()));
    Simulation::k_d = value/100.0f;
    Object::setKd(Simulation::k_d);
}

void Simulation::on_ScenaryType_currentIndexChanged(int index)
{
    if (initiatedScen > 1) {
        Simulation::scenaryType = static_cast<ScenaryType>(index);
        ui->openGLWidget->cleanScenary();
        usleep(1000000);
        ui->openGLWidget->loadScenary();
    }
    else{
        ++initiatedScen;
    }
}


void Simulation::on_SolverMethod_currentIndexChanged(int index)
{
    if (initiatedSolv > 1) {
        Simulation::solverType = static_cast<Object::SolverType>(index);
        Object::setSolverModel(solverType);
    }
    else{
        ++initiatedSolv;
    }
}

