#ifndef OBJECT_H
#define OBJECT_H

#include <eigen3/Eigen/Geometry>
#include <QEvent>
#include <list>
#include "camera.h"
#include "utils.h"

#define QTTYSHADERS 2

class Object {

public:
    enum SolverType  {Euler, SemiEuler, Verlet, RungeKuta2};
    enum PhysicsType {Normal, Immovable, Transparent};
    enum ShaderType  {Vanilla = 0, Sphere = 1};

    virtual ~Object();
    virtual void render() const = 0;
    virtual void event(QEvent *event) = 0;

    void update(const double deltatime, const std::list<Object*>& meshs);
    virtual void forceUpdate() = 0;

    static void cameraMatrixCalc(data_visualization::Camera camera_);
    static bool vanillaProgramsLoad();
    static void deleteVanillas();

    void load();
    virtual bool isColliding(Object& object) const = 0;

    void correctObject(const Eigen::Vector3d& n, const double d, bool add);

    virtual double getRadius() const = 0;
    double getWeight() const;
    double getIWeight() const;
    double getElasticity() const;
    double getFriction() const;
    Eigen::Vector3d getPosition() const;
    Eigen::Vector3d getPassPosition() const;
    Eigen::Vector3d getVelocity() const;
    Eigen::Vector3d getPassVelocity() const;

    static void setSolverModel(const SolverType solverType);
    static void setGravityScale(const double scale);
    static void setKd(double kd);

protected:
    // openGL basic shaders and render vars
    inline static Eigen::Matrix4d projection, view, model, t, normal;
    inline static std::vector<unsigned int> programsList;

    unsigned int VAO, VBO, EBO, qttyFaces;
    Eigen::Vector3d objectColor;
    std::vector<double> vertices;
    std::vector<unsigned int> indices;

    static int loadShader(const char * vertex, const char *frag, const char *geo);
    void renderType(int type) const;

    // phisics variables
    inline static double k_d;
    inline static double gravityScale = GENERAL_GRAVITY_SCALE;
    inline static Eigen::Vector3d gravity = gravityScale * GENERAL_GRAVITY;

    Eigen::Vector3d p, p_pass, v, v_pass, f;
    double w_i, m, e, u;
    inline static SolverType solverType;
    PhysicsType physicsType = PhysicsType::Normal;
    ShaderType  shaderType;

    virtual bool collisionDetect(const std::list<Object*>& objects) = 0;
    virtual void propagateCollision(const std::list<Object*>& meshs) = 0;
    void solver(const double dt);
    void initSolver();

private:
    inline static Eigen::Vector3d lightColor = GENERAL_LIGHT;

};

#endif // OBJECT_H
