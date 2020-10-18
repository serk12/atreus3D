#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <GL/glew.h>

#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QTimer>
#include <list>

#include "fpscount.h"
#include "camera.h"
#include "object.h"
#include "mesh.h"
#include "particle.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

    void cleanScenary();
    void loadScenary();
    void setBirthTime(int value);
    void updeteElasticityTerms(float k_elas);
    void updeteDumpingTerms(float k_dump);
    void updeteDistanceTerms(float d);

public slots:
    void cleanup();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void createParticles();
    //FPS limiter + counter
    void updateFPS();
    QElapsedTimer frameTime;
    QTimer *fpsTimer, *birdTimer;
    float previousTime = 0.0f;
    int frameCounter = 0;

    //OpenGL initialization
    data_visualization::Camera camera_;
    float width_, height_;
    bool initialized = false;

    //objects = meshes, particles
    std::pair<std::list<Mesh*>, std::list<Particle*> > objects;
    std::list<QTimer*> lifeTimer;


    int toCreate = 0;

private slots:
    void killParticle();
    void createParticle();
};



#endif // GLWIDGET_H
