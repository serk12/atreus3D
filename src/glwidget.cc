#include "glwidget.h"

#include <eigen3/Eigen/Geometry>
#include <fstream>
#include <iostream>

#include "mesh.h"
#include "particle.h"
#include "simulation.h"

const double kFieldOfView = 60;
const double kZNear = 0.0001;
const double kZFar = 10;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    fpsTimer = new QTimer(this);
    connect(fpsTimer, SIGNAL(timeout()), this, SLOT(update()));
    fpsTimer->start(10);

    birdTimer = new QTimer(this);
    connect(birdTimer, SIGNAL(timeout()), this, SLOT(createParticle()));
    birdTimer->start(Simulation::birdTime);

    frameTime.start();

}

GLWidget::~GLWidget()
{
    cleanup();
    Object::deleteVanillas();
    delete fpsTimer;
    for (Mesh* &m : objects.first) {
        delete m;
    }
    for (Particle* &p : objects.second) {
        delete p;
    }
}

void GLWidget::cleanup()
{
    makeCurrent();
    doneCurrent();
}


void GLWidget::initializeGL()
{
    glewInit();

    glEnable(GL_NORMALIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);

    bool no_problems = Object::vanillaProgramsLoad();
    if (! no_problems) exit(0);
    loadScenary();
    initialized = true;
}

void GLWidget::updateFPS()
{
    ++frameCounter;
    if (frameTime.elapsed() >= 1000) {
        double fps = frameCounter / ((double)frameTime.elapsed()/1000.0);
        FPSCount::setFPS(int(fps));
        frameTime.start();
        previousTime = frameCounter = 0;
    }
}

void GLWidget::createParticles()
{
    if (objects.second.size() < Simulation::maxParticles) {
        for (int i = 0; i < toCreate; ++i) {
            Simulation::addParticle(this->objects.second);
            QTimer *liveTimer = new QTimer(this);
            connect(liveTimer, SIGNAL(timeout()), this, SLOT(killParticle()));
            liveTimer->start(Simulation::liveTime);
            lifeTimer.push_back(liveTimer);
        }
    }
    toCreate = 0;
}

void GLWidget::paintGL()
{
    createParticles();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (initialized) {
        Object::cameraMatrixCalc(camera_);
        float currentTime = frameTime.elapsed();
        float dt = currentTime - previousTime;
        //dt = 3;
        previousTime = currentTime;

        //mesh
        std::list<Object*> aux(objects.first.begin(), objects.first.end());
        for (Mesh* &o : objects.first) {
            o->update(dt, aux);
            o->render();
        }
        //particles
        for (Particle* &o : objects.second) {
            o->forceUpdate();
        }
        for (Particle* &o : objects.second) {
            o->update(dt, aux);
            o->render();
        }
    }
    updateFPS();
}

void GLWidget::resizeGL(int w, int h)
{
    width_ = w;
    height_ = (h == 0)? 1 : h;

    camera_.SetViewport(0, 0, w, h);
    camera_.SetProjection(kFieldOfView, kZNear, kZFar);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        camera_.StartRotating(event->x(), event->y());
        break;
    case Qt::RightButton:
        camera_.StartZooming(event->x(), event->y());
        break;
    default:
        break;
    }

    update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    camera_.SetRotationX(event->y());
    camera_.SetRotationY(event->x());
    camera_.SafeZoom(event->y());
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    switch (event->button()) {
    case Qt::LeftButton:
        camera_.StopRotating(event->x(), event->y());
        break;
    case Qt::RightButton:
        camera_.StopZooming(event->x(), event->y());
        break;
    default:
        break;
    }

    update();
}

void GLWidget::killParticle()
{
    if (this->objects.second.size() > 0) {
        Object *a = this->objects.second.front();
        this->objects.second.pop_front();
        delete a;
        QTimer *liveTimer = lifeTimer.front();
        delete liveTimer;
        lifeTimer.pop_front();
    }
    else {
        lifeTimer.clear();
    }
}

void GLWidget::createParticle()
{
    if (objects.second.size() + toCreate < Simulation::maxParticles) {
        ++toCreate;
    }
}

void GLWidget::cleanScenary()
{
    toCreate = 0;
    for (QTimer* &t : lifeTimer) {
        delete t;
    }
    lifeTimer.clear();

    for (Mesh* &m : objects.first) {
        delete m;
    }
    objects.first.clear();

    for (Particle* &p : objects.second) {
        delete p;
    }
    objects.second.clear();
    initialized = false;
}


void GLWidget::loadScenary() {
    Simulation::loadSim(objects);
    for (Mesh* &m : objects.first) {
        m->load();
    }
    for (Particle* &p : objects.second) {
        p->load();
    }
    initialized = true;
}

void GLWidget::setBirthTime(int value) {
    birdTimer->start(value);
}


void GLWidget::updeteElasticityTerms(float k_elas) {
    for (Particle* &p : objects.second) {
        p->setElasticityTerm(k_elas);
    }
}


void GLWidget::updeteDumpingTerms(float k_dump) {
    for (Particle* &p : objects.second) {
        p->setDampingTerm(k_dump);
    }
}

void GLWidget::updeteDistanceTerms(float d) {
    for (Particle* &p : objects.second) {
        p->setDistancyTerm(d);
    }
}
