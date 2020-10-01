#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <GL/glew.h>

#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QTimer>
#include <vector>

#include "fpscount.h"
#include "camera.h"
#include "object.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

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
    //FPS limiter + counter
    void updateFPS();
    QElapsedTimer frameTime;
    QTimer* fpsTimer;
    int frameCounter = 0;

    //OpenGL initialization
    data_visualization::Camera camera_;
    float width_, height_;
    bool initialized = false;

    //objects = meshes || particles
    std::vector<Object*> objects;

};



#endif // GLWIDGET_H
