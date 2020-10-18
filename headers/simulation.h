#ifndef SIMULATION_H
#define SIMULATION_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Simulation; }
QT_END_NAMESPACE

#include "object.h"
#include "particle.h"
#include "mesh.h"
#include "utils.h"
#include <list>
#include <unistd.h>

class Simulation : public QMainWindow
{
    Q_OBJECT
public:
    enum ScenaryType {Cascade, Rain, Fountain, String, Debug, DebugS};

    Simulation(QWidget *parent = nullptr);
    ~Simulation();

    static bool loadSim(std::pair<std::list<Mesh*>, std::list<Particle*> >& objects);
    static void addParticle(std::list<Particle*>& particleList);

    inline static ScenaryType scenaryType = Simulation::ScenaryType::String;
    inline static Object::SolverType solverType = Object::SolverType::Verlet;

    inline static unsigned int liveTime = GENERAL_LIVE_TIME;
    inline static unsigned int birdTime = GENERAL_BIRTH_TIME;
    inline static unsigned int maxParticles = GENERAL_MAX_PARTICLES;

    inline static float gravityScale = GENERAL_GRAVITY_SCALE;
    inline static float k_d = GENERAL_K_D;

    inline static float e = GENERAL_E;
    inline static float u = GENERAL_U;
    inline static float m = GENERAL_M;

    inline static float k_elas = GENERAL_K_ELAS;
    inline static float k_dump =  GENERAL_K_DUMP;
    inline static float d = GENERAL_D;

    inline static int initiatedScen = 0;
    inline static int initiatedSolv = 0;

private:
    Ui::Simulation *ui;

private slots:
    void on_GravityScale_valueChanged(int value);
    void on_TopParticles_valueChanged(int value);
    void on_BirdTime_valueChanged(int value);
    void on_LiveTime_valueChanged(int value);
    void on_ScenaryType_currentIndexChanged(int index);
    void on_SolverMethod_currentIndexChanged(int index);
    void on_Kd_valueChanged(int value);
    void on_Plastic_valueChanged(double e);
    void on_Friction_valueChanged(double u);
    void on_Mass_valueChanged(double m);
    void on_Elasticity_valueChanged(double k_elas);
    void on_Damping_valueChanged(double k_dump);
    void on_Distance_valueChanged(double d);
};

#endif // SIMULATION_H
