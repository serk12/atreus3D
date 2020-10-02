#include "glwidget.h"

#include <eigen3/Eigen/Geometry>
#include <fstream>
#include <iostream>
#include "mesh.h"

const double kFieldOfView = 60;
const double kZNear = 0.0001;
const double kZFar = 10;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    fpsTimer = new QTimer(this);
    connect(fpsTimer , SIGNAL(timeout()), this, SLOT(update()));
    fpsTimer->start(10);

    frameTime.start();

    Mesh *a = new Mesh();
    objects.push_back(a);
    create();
}

GLWidget::~GLWidget()
{
    cleanup();
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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    bool no_problems = Object::vanillaProgramLoad();
    for (Object* &o : objects) {
        no_problems = no_problems && o->load();
    }
    if (! no_problems) exit(0);

    initialized = true;
}

void GLWidget::updateFPS()
{
    ++frameCounter;
    if (frameTime.elapsed() >= 1000) {
        double fps = frameCounter / ((double)frameTime.elapsed()/1000.0);
        FPSCount::setFPS(int(fps));
    }
}

void GLWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (initialized) {
        camera_.SetViewport();
        Object::cameraMatrixCalc(camera_);
        for (Object* &o : objects) {
            o->render();
        }
    }
    updateFPS();
}

void GLWidget::resizeGL(int w, int h)
{
    if (h == 0) h = 1;
    width_ = w;
    height_ = h;

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

