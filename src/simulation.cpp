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
    ui->SolverMethod->insertItem(int(Object::SolverType::Euler), "Euler", Object::SolverType::Euler);
    ui->SolverMethod->insertItem(int(Object::SolverType::SemiEuler), "SemiEuler", Object::SolverType::SemiEuler);
    ui->SolverMethod->insertItem(int(Object::SolverType::Verlet), "Verlet", Object::SolverType::Verlet);

    ui->ScenaryType->insertItem(int(ScenaryType::Cascade), "Cascade", ScenaryType::Cascade);
    ui->ScenaryType->insertItem(int(ScenaryType::Rain), "Rain", ScenaryType::Rain);
    ui->ScenaryType->insertItem(int(ScenaryType::Fountain), "Fountain", ScenaryType::Fountain);
    ui->ScenaryType->insertItem(int(ScenaryType::String), "String", ScenaryType::String);
    ui->ScenaryType->insertItem(int(ScenaryType::Debug), "Debug", ScenaryType::Debug);
    ui->ScenaryType->insertItem(int(ScenaryType::DebugS), "Debug String", ScenaryType::DebugS);
    ui->ScenaryType->insertItem(int(ScenaryType::CurlyString), "Curly String", ScenaryType::CurlyString);
    ui->ScenaryType->insertItem(int(ScenaryType::Cloth), "Cloth", ScenaryType::Cloth);
    ui->ScenaryType->insertItem(int(ScenaryType::RigidBody), "Rigid Body", ScenaryType::Cloth);

    ui->BirdTime->setValue(Simulation::birdTime);
    ui->Kd->setValue(Simulation::k_d*100);
    ui->LiveTime->setValue(Simulation::liveTime);
    ui->TopParticles->setValue(Simulation::maxParticles);
    ui->GravityScale->setValue(Simulation::gravityScale*100);

    ui->SolverMethod->setCurrentIndex(int(Simulation::solverType));
    ui->ScenaryType->setCurrentIndex(int(Simulation::scenaryType));

    ui->Distance->setValue(Simulation::d);
    ui->Elasticity->setValue(Simulation::k_elas);
    ui->Damping->setValue(Simulation::k_dump);

    ui->Mass->setValue(Simulation::m);
    ui->Plastic->setValue(Simulation::e);
    ui->Friction->setValue(Simulation::u);
}

Simulation::~Simulation()
{
    delete ui;
}

void defaultBox(std::pair<std::list<Mesh*>, std::list<Particle*> >& objects)
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
}

void defaultPlane(std::pair<std::list<Mesh*>, std::list<Particle*> >& objects)
{
    float rx = 0.75f;
    float ry = 0.3f;
    float rz = 0.75f;
    std::vector<float> box;
    std::vector<unsigned int> boxi;
    box = {
        -1.0f*rx, -1.0f*ry,  1.0f*rz,// 6
        1.0f*rx, -1.0f*ry,  1.0f*rz, // 2
        1.0f*rx, -1.0f*ry, -1.0f*rz, // 3
       -1.0f*rx, -1.0f*ry, -1.0f*rz, // 7
    };

    boxi = {0,1,2,3};
    Plane *a = new Plane(box, boxi, Object::ShaderType::Vanilla, Eigen::Vector3f(0.0f, 1.0f, 0.0f), Eigen::Vector3f(0.0f, -1.00f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), -1, 0.95f, 0.80f,  GL_LINE_LOOP);
    objects.first.push_back(a);
}


bool CubeScene(std::pair<std::list<Mesh*>, std::list<Particle*> >& objects)
{
    defaultBox(objects);

    std::vector<float> box;
    std::vector<unsigned int> boxi;
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

    Sphere *s = new Sphere(Eigen::Vector3f(0.2f,0.5f,1.0f),  Eigen::Vector3f(0.0f,0.0f,0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), -1, 0.95f, 0.80f, 0.18f);
    objects.first.push_back(s);

    return true;
}

void createString(std::list<Particle*>& particles, Eigen::Vector3f pos, int qttyPar, int id)
{
    Particle* aux = new Particle(pos, Eigen::Vector3f(0.0f,0.0f,0.0f), -1.0f, Simulation::e, Simulation::u);
    particles.push_back(aux);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    float r = dis(gen)-0.5f;
    for (int i = 1; i < qttyPar; ++i) {
        Particle* c = new Particle(pos + Eigen::Vector3f(0.0f, Simulation::d*i, 0.0f),
                                   Eigen::Vector3f(0.001f*r, 0.0f*r, 0.001f*r),
                                   Simulation::m, Simulation::e, Simulation::u, Eigen::Vector3f(0.1f*i, id/10.0f, 0.1f*i));
        particles.push_back(c);
        c->addParticle(aux, Simulation::d);
        aux->addParticle(c, Simulation::d);
        aux = c;
    }

    Particle* c = new Particle(pos + Eigen::Vector3f(0.0f, Simulation::d*qttyPar, 0.0f),
                               Eigen::Vector3f(0.0f,0.0f,0.0f),
                               Simulation::m, Simulation::e, Simulation::u);
    c->addParticle(aux, Simulation::d);
    aux->addParticle(c, Simulation::d);
    particles.push_back(c);
}

void creatCurlyHair(std::list<Particle*>& particles, Eigen::Vector3f pos, int qttyPar, int id)
{
    Particle* aux = new Particle(pos, Eigen::Vector3f(0.0f,0.0f,0.0f), -1.0f, Simulation::e, Simulation::u);
    particles.push_back(aux);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    float r = dis(gen)-0.5f;
    Particle *aux2 = nullptr;
    Particle *aux3 = nullptr;

    for (int i = 1; i < qttyPar; ++i) {
        Particle* c = new Particle(pos + Eigen::Vector3f(0.0f, Simulation::d*i, 0.0f),
                                   Eigen::Vector3f(0.001f*r, 0.0f*r, 0.001f*r),
                                   Simulation::m, Simulation::e, Simulation::u, Eigen::Vector3f(0.1f*i, id/10.0f, 0.1f*i));
        particles.push_back(c);
        if(aux3 != nullptr) {
            c->addParticle(aux3, Simulation::d*1.5f);
            aux3->addParticle(c, Simulation::d*1.5f);
        }
        aux3 = aux2;
        if(aux2 != nullptr) {
            c->addParticle(aux2, Simulation::d*1.25f);
            aux2->addParticle(c, Simulation::d*1.25f);

        }
        aux2 = aux;
        c->addParticle(aux, Simulation::d);
        aux->addParticle(c, Simulation::d);
        aux = c;
    }

    Particle* c = new Particle(pos + Eigen::Vector3f(0.0f, Simulation::d*qttyPar, 0.0f),
                               Eigen::Vector3f(0.0f,0.0f,0.0f),
                               Simulation::m, Simulation::e, Simulation::u);
    c->addParticle(aux, Simulation::d);
    aux->addParticle(c, Simulation::d);
    particles.push_back(c);
}

bool stringScene(std::pair<std::list<Mesh*>, std::list<Particle*> >&objects)
{
    defaultPlane(objects);

    Sphere *s = new Sphere(Eigen::Vector3f(0.2f,0.5f,1.0f),
                           Eigen::Vector3f(0.0f,0.2f,0.1f),
                           Eigen::Vector3f(0.0f,0.0f,0.0f), -1, 0.95f, 0.80f, 0.5f);
    objects.first.push_back(s);

    int qttyStrings = 3, qttyPar = 10;
    float gap = 0.15f;
    for (int j = 0; j < qttyStrings; ++j) {
        createString(objects.second, Eigen::Vector3f((-qttyStrings*gap*0.5f) + gap*j,1.0f, 0.0f), qttyPar, j);
    }
    return true;
}

bool stringCurly(std::pair<std::list<Mesh*>, std::list<Particle*> >&objects)
{
    defaultPlane(objects);

    int qttyStrings = 3, qttyPar = 10;
    float gap = 0.15f;
    for (int j = 0; j < qttyStrings; ++j) {
        creatCurlyHair(objects.second, Eigen::Vector3f((-qttyStrings*gap*0.5f) + gap*j,1.0f, 0.0f), qttyPar, j);
    }
    return true;
}

void generateCloth(std::list<Particle*>& particles, int maxI, int maxJ) {
    std::vector< std::vector<Particle* > > particlesMesh(maxI, std::vector<Particle*>(maxJ));
    for (int i = 0; i < maxI; ++i) {
        for (int j = 0; j < maxJ; ++j) {
            // float m_aux = Simulation::m;
            // float m_aux = i == (maxI-1)? -1 : Simulation::m;
            float m_aux = (i == (maxI-1) and (j == (maxJ-1) or j == 0))? -1 : Simulation::m;
            particlesMesh[i][j] = new Particle(Eigen::Vector3f(-Simulation::d*maxJ*0.5f + Simulation::d*j*1.0f,  1.0f,-Simulation::d*maxI*0.5f + Simulation::d*i*1.0f),
                                               Eigen::Vector3f(0.0f,0.0f,0.0f),
                                               m_aux, Simulation::e, Simulation::u);
        }
    }
    for (int i = 0; i < maxI; ++i) {
        for (int j = 0; j < maxJ; ++j) {
            Particle* c = particlesMesh[i][j];
            particles.push_back(c);
            //cross
            if(i - 1 >= 0) {
                c->addParticle(particlesMesh[i - 1][j], Simulation::d);
            }
            if(i + 1 < maxI) {
                c->addParticle(particlesMesh[i + 1][j], Simulation::d);
            }
            if(j - 1 >= 0) {
                c->addParticle(particlesMesh[i][j - 1], Simulation::d);
            }
            if(j + 1 < maxJ) {
                c->addParticle(particlesMesh[i][j + 1], Simulation::d);
            }

            //2*cross
            if(i - 2 >= 0) {
                c->addParticle(particlesMesh[i - 2][j], Simulation::d*2.0f);
            }
            if(i + 2 < maxI) {
                c->addParticle(particlesMesh[i + 2][j], Simulation::d*2.0f);
            }
            if(j - 2 >= 0) {
                c->addParticle(particlesMesh[i][j - 2], Simulation::d*2.0f);
            }
            if(j + 2 < maxJ) {
                c->addParticle(particlesMesh[i][j + 2], Simulation::d*2.0f);
            }

            //diagonal
            if(i - 1 >= 0 and j - 1 >= 0) {
                c->addParticle(particlesMesh[i - 1][j - 1], Simulation::d*1.4143f);
            }
            if(i + 1 < maxI and j + 1 < maxJ) {
                c->addParticle(particlesMesh[i + 1][j + 1], Simulation::d*1.4143f);
            }
            if(j - 1 >= 0 and i + 1 < maxI) {
                c->addParticle(particlesMesh[i + 1][j - 1], Simulation::d*1.4143f);
            }
            if(j + 1 < maxJ and i - 1 >= 0) {
                c->addParticle(particlesMesh[i - 1][j + 1], Simulation::d*1.4143f);
            }
        }
    }
}

bool createCloth(std::pair<std::list<Mesh*>, std::list<Particle*> >&objects)
{
    defaultPlane(objects);

    Sphere *s = new Sphere(Eigen::Vector3f(0.2f,0.5f,1.0f),
                           Eigen::Vector3f(0.0f,0.25f,0.0f),
                           Eigen::Vector3f(0.0f,0.0f,0.0f), -1, 0.95f, 0.80f, 0.30f);
    objects.first.push_back(s);

    generateCloth(objects.second, 10, 10);

    return true;
}

bool createRigidBody(std::pair<std::list<Mesh*>, std::list<Particle*> >&objects) {
    //Polygon *p = new Polygon("../atreus3D/models/Patricio.obj" , Eigen::Vector3f(9.5f,-6.0f,0.0f), 10.0f, Eigen::Vector3f(0.0f,0.0f,0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), 1);
    //objects.first.push_back(p);
    std::vector<float> box;
    std::vector<unsigned int> boxi;
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
    Triangle *t = new Triangle(box, boxi, Eigen::Vector3f(0.0f, 0.12f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), 2.0f);
    objects.first.push_back(t);
    Sphere *s = new Sphere(Eigen::Vector3f(0.2f,0.5f,1.0f),  Eigen::Vector3f(0.0f,0.0f,0.0f), Eigen::Vector3f(0.0f,0.0f,0.0f), 3, 0.95f, 0.50f, 0.18f);
    objects.first.push_back(s);
    defaultBox(objects);
    return true;
}


bool Simulation::loadSim(std::pair<std::list<Mesh*>, std::list<Particle*> >& objects)
{
    Object::setSolverModel(Simulation::solverType);
    Object::setKd(Simulation::k_d);
    Object::setGravityScale(Simulation::gravityScale);
    bool result;
    Simulation::liveTime = 1000000000;
    switch (Simulation::scenaryType) {
    case ScenaryType::DebugS:
    case ScenaryType::String:
        result = stringScene(objects);
        break;
    case ScenaryType::CurlyString:
        result = stringCurly(objects);
        break;
    case ScenaryType::Cloth:
        result = createCloth(objects);
        break;
    case ScenaryType::RigidBody:
        Simulation::liveTime = GENERAL_LIVE_TIME;
        result = createRigidBody(objects);
        break;
    case ScenaryType::Cascade:
    case ScenaryType::Fountain:
    case ScenaryType::Debug:
    case ScenaryType::Rain:
    default:
        Simulation::liveTime = GENERAL_LIVE_TIME;
        result = CubeScene(objects);
        break;
    }
    return result;
}

void Simulation::addParticle(std::list<Particle*>& particleList)
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
    case ScenaryType::RigidBody:
    case ScenaryType::Rain:
        b = new Particle(Eigen::Vector3f(rx*0.5f, 0.85f, rz*0.5f), Eigen::Vector3f(0.0f,0.0f,0.0f), m, e, u);
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
    std::string text = "Max Particles: #" + std::to_string(value);
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
    if (initiatedScen == IndexType::OPEN) {
        initiatedScen = IndexType::BLOCK;
        Simulation::scenaryType = static_cast<ScenaryType>(index);
        ui->openGLWidget->cleanScenary();
        ui->openGLWidget->loadScenary();
        initiatedScen = IndexType::OPEN;
    }
    else if (initiatedScen == IndexType::LOAD){
        initiatedScen = IndexType::PRELOAD;
    }
    else if (initiatedScen == IndexType::PRELOAD) {
        initiatedScen = IndexType::OPEN;
    }
}


void Simulation::on_SolverMethod_currentIndexChanged(int index)
{
    if (initiatedSolv == IndexType::OPEN) {
        initiatedSolv = IndexType::BLOCK;
        Simulation::solverType = static_cast<Object::SolverType>(index);
        Object::setSolverModel(solverType);
        initiatedSolv = IndexType::LOAD;
    }
    else if (initiatedSolv == IndexType::LOAD){
        initiatedSolv = IndexType::PRELOAD;
    }
    else if (initiatedSolv == IndexType::PRELOAD) {
        initiatedSolv = IndexType::OPEN;
    }
}


void Simulation::on_Plastic_valueChanged(double e)
{
    Simulation::e = e;
}

void Simulation::on_Friction_valueChanged(double u)
{
    Simulation::u = u;
}

void Simulation::on_Mass_valueChanged(double m)
{
    Simulation::m = m;
}

void Simulation::on_Elasticity_valueChanged(double k_elas)
{
    ui->openGLWidget->updeteElasticityTerms(k_elas);
}

void Simulation::on_Damping_valueChanged(double k_damp)
{
    ui->openGLWidget->updeteDumpingTerms(k_damp);
}

void Simulation::on_Distance_valueChanged(double d)
{
    ui->openGLWidget->updeteDistanceTerms(d);
}
