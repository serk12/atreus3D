#ifndef OBJECT_H
#define OBJECT_H

#include <eigen3/Eigen/Geometry>
#include <QEvent>
#include "camera.h"

enum SolverType  {euler = 0, semiEuler = 1, verlet = 2, rungeKuta2 = 3};
enum PhysicsType {normal = 0, immovable = 1, transparent = 2};
class Object {

public:
    ~Object();
    virtual void render() const = 0;
    virtual void event(QEvent *event) = 0;

    void update(const float deltatime);

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

    Eigen::Vector3f p, p_pass, v, f_final;
    float w_i, m;
    SolverType solverType = SolverType::euler;
    PhysicsType physicsType = PhysicsType::normal;

    virtual void forceUpdate() = 0;
    virtual void collisionDetect() = 0;
    virtual bool possitionCorrect() = 0;
    void solver(const float dt);
    void initSolver();

private:
    inline static Eigen::Vector3f lightColor = Eigen::Vector3f(1.0f, 1.0f, 1.0f);;

};

#endif // OBJECT_H
