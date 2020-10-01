#ifndef OBJECT_H
#define OBJECT_H

#include <eigen3/Eigen/Geometry>
#include <QOpenGLShaderProgram>
#include <QEvent>
#include <vector>

#include "camera.h"
enum ProgramVars {projMatrix = 0, mvMatrix = 1, normalMatrix = 2, lightPos = 3};

class Object {

public:
    void render() const;
    virtual void update(const float deltatime) = 0;
    virtual void event(QEvent *event) = 0;
    virtual bool load() = 0;

    static void cameraMatrixCalc(data_visualization::Camera camera_);
    static bool vanillaProgramLoad();

protected:
    static Eigen::Matrix4f projection, view, model, t, normal;

    static unsigned int vanilla;

    unsigned int VAO, program = 0, qttyFaces;
    std::vector<ProgramVars> programVars;

private:

};

#endif // OBJECT_H
