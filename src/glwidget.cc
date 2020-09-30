#include "glwidget.h"

const char vertexShaderSource[] = "../atreus3D/shaders/vanilla.vert";
const char fragmentShaderSource[] = "../atreus3D/shaders/vanilla.frag";

const double kFieldOfView = 60;
const double kZNear = 0.0001;
const double kZFar = 10;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    fpsTimer = new QTimer(this);
    connect(fpsTimer , SIGNAL(timeout()), this, SLOT(update()));
    fpsTimer->start(10);
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
    initializeOpenGLFunctions();

    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::paintGL()
{
    ++frameCounter;
    if (frameTime.elapsed() >= 1000) {
        double fps = frameCounter / ((double)frameTime.elapsed()/1000.0);
        FPSCount::setFPS(int(fps));
      }
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
    if (event->button() == Qt::LeftButton) {
      camera_.StopRotating(event->x(), event->y());
    }
    if (event->button() == Qt::RightButton) {
      camera_.StopZooming(event->x(), event->y());
    }
    update();

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
      camera_.StartRotating(event->x(), event->y());
    }
    if (event->button() == Qt::RightButton) {
      camera_.StartZooming(event->x(), event->y());
    }
    update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    camera_.StopRotating(event->x(), event->y());
  }
  if (event->button() == Qt::RightButton) {
    camera_.StopZooming(event->x(), event->y());
  }
  update();
}

