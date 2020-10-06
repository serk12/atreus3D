#ifndef OBJECT_H
#define OBJECT_H

#include <eigen3/Eigen/Geometry>
#include <QEvent>
#include <list>
#include "camera.h"

#define QTTYSHADERS 2

class Object {

public:
    enum SolverType  {Euler = 0, SemiEuler = 1, Verlet = 2, RungeKuta2 = 3};
    enum PhysicsType {Normal = 0, Immovable = 1, Transparent = 2};
    enum ShaderType  {Vanilla = 0, Sphere = 1};

    ~Object();
    virtual void render() const = 0;
    virtual void event(QEvent *event) = 0;

    void update(const float deltatime, const std::list<Object*>& meshs);

    static void cameraMatrixCalc(data_visualization::Camera camera_);
    static bool vanillaProgramsLoad();
    static void deleteVanillas();

    void load();


protected:
    // openGL basic shaders and render vars
    inline static Eigen::Matrix4f projection, view, model, t, normal;
    inline static std::vector<unsigned int> programsList;

    unsigned int VAO, VBO, EBO, qttyFaces, program;
    Eigen::Vector3f objectColor;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    static int loadShader(const char * vertex, const char *frag, const char *geo);
    void renderType(int type) const;

    // phisics variables
    inline static const float k_d = 0.975f;
    inline static const float gravityScale = 0.00000000005f;
    inline static const Eigen::Vector3f gravity = gravityScale * Eigen::Vector3f(0.0f, -9.81f, 0.0f);

    Eigen::Vector3f p, p_pass, v, f;
    float w_i, m;
    SolverType  solverType  = SolverType::Euler;
    PhysicsType physicsType = PhysicsType::Normal;

    virtual void forceUpdate() = 0;
    virtual void collisionDetect(const std::list<Object*>& objects) = 0;
    virtual bool possitionCorrect() = 0;
    void solver(const float dt);
    void initSolver();

private:
    inline static Eigen::Vector3f lightColor = Eigen::Vector3f(1.0f, 1.0f, 1.0f);;

};

#endif // OBJECT_H
