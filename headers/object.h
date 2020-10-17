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

    virtual ~Object();
    virtual void render() const = 0;
    virtual void event(QEvent *event) = 0;

    void update(const float deltatime, const std::list<Object*>& meshs);
    virtual void forceUpdate() = 0;

    static void cameraMatrixCalc(data_visualization::Camera camera_);
    static bool vanillaProgramsLoad();
    static void deleteVanillas();

    void load();
    virtual bool isColliding(Object& object) const = 0;

    void correctParticle(const Eigen::Vector3f& n, const float d);

    virtual float getRadius() const = 0;
    float getWeight() const;
    float getIWeight() const;
    float getElasticity() const;
    float getFriction() const;
    Eigen::Vector3f getPosition() const;
    Eigen::Vector3f getPassPosition() const;
    Eigen::Vector3f getVelocity() const;

    static void setSolverModel(const SolverType solverType);
    static void setGravityScale(const float scale);

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
    inline static const float k_d = 0.95f;
    inline static float gravityScale = 1.0f;
    inline static Eigen::Vector3f gravity = gravityScale * Eigen::Vector3f(0.0f, -9.81f, 0.0f);

    Eigen::Vector3f p, p_pass, v, v_pass, f;
    float w_i, m, e, u;
    inline static SolverType solverType = SolverType::SemiEuler;
    PhysicsType physicsType = PhysicsType::Normal;
    ShaderType  shaderType;

    virtual void collisionDetect(const std::list<Object*>& objects) = 0;
    virtual bool possitionCorrect() = 0;
    void solver(const float dt);
    void initSolver();

private:
    inline static Eigen::Vector3f lightColor = Eigen::Vector3f(1.0f, 1.0f, 1.0f);

};

#endif // OBJECT_H
