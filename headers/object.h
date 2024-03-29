#ifndef OBJECT_H
#define OBJECT_H

#include <eigen3/Eigen/Geometry>
#include <QEvent>
#include <list>
#include <iostream>

#include "camera.h"
#include "utils.h"

#define QTTYSHADERS 2

class Object {

public:
    inline static bool const POSITIVE_OFFSET = true;
    inline static bool const NEGATIVE_OFFSET = false;
    inline static bool const POSITIVE_FORCE = true;
    inline static bool const NEGATIVE_FORCE = false;
    enum ObjectType  {_Particle, _Polygon, _Plane, _Sphere, _Triangle};
    enum SolverType  {Euler, SemiEuler, Verlet, RungeKuta2};
    enum PhysicsType {Normal, Immovable, Transparent};
    enum ShaderType  {Vanilla = 0, Ball = 1};

    virtual ~Object();
    virtual void render() const = 0;
    virtual void event(QEvent *event) = 0;

    void update(const float deltatime, const std::list<Object*>& meshs);
    virtual void forceUpdate() = 0;

    static void cameraMatrixCalc(data_visualization::Camera camera_);
    static bool vanillaProgramsLoad();
    static void deleteVanillas();

    void load();
    virtual bool isColliding(Object& object) = 0;

    void correctObject(const Eigen::Vector3f& n, const float d, bool offset, bool sign);

    virtual float getRadius() const = 0;
    virtual float getRadiusSqrt() const = 0;
    virtual Eigen::Matrix3f getInertiaMatrix() const;

    float getWeight() const;
    float getIWeight() const;
    float getElasticity() const;
    float getFriction() const;
    Eigen::Vector3f getPosition() const;
    Eigen::Vector3f getPassPosition() const;
    Eigen::Vector3f getVelocity() const;
    Eigen::Vector3f getPassVelocity() const;

    static void setSolverModel(const SolverType solverType);
    static void setGravityScale(const float scale);
    static void setKd(float kd);
    ObjectType getType() const;
    unsigned int getID() const;

protected:
    // openGL basic shaders and render vars
    inline static Eigen::Matrix4f projection, view, model, t, normal;
    inline static std::vector<unsigned int> programsList;

    unsigned int VAO, VBO, EBO, qttyFaces;
    Eigen::Vector3f objectColor;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    static int loadShader(const char * vertex, const char *frag, const char *geo);
    void renderType(int type) const;

    // phisics variables
    inline static float k_d;
    inline static float gravityScale = GENERAL_GRAVITY_SCALE;
    inline static Eigen::Vector3f gravity = gravityScale * GENERAL_GRAVITY;

    Eigen::Vector3f p, p_pass, v, v_pass, P, L, tor, f;
    Eigen::Matrix3f I_inv, I_body;
    Eigen::Quaternionf q;
    float w_i, m, e, u;
    inline static SolverType solverType;
    PhysicsType physicsType = PhysicsType::Normal;
    ShaderType  shaderType;

    bool collisionDetect(const std::list<Object*>& objects);
    virtual void propagateCollision(const std::list<Object*>& meshs) = 0;
    void solver(const float dt);
    void initSolver();

    ObjectType _type;

private:
    inline static Eigen::Vector3f lightColor = GENERAL_LIGHT;

};

#endif // OBJECT_H
