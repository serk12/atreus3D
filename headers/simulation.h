#ifndef SIMULATION_H
#define SIMULATION_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Simulation; }
QT_END_NAMESPACE

#include "object.h"
#include <list>
#include <unistd.h>

class Simulation : public QMainWindow
{
    Q_OBJECT
public:
    enum ScenaryType {Cascade, Rain, Fountain, String, Debug, DebugS};

    Simulation(QWidget *parent = nullptr);
    ~Simulation();

    static bool loadSim(std::pair<std::list<Object*>, std::list<Object*> >& objects);
    static void addParticle(std::list<Object*>& particleList);

    inline static ScenaryType scenaryType = ScenaryType::String;
    inline static Object::SolverType solverType = Object::SolverType::Verlet;

    inline static unsigned int liveTime = 100000;
    inline static unsigned int birdTime = 101;
    inline static unsigned int maxParticles = 501;

    inline static float gravityScale = .990f;
    inline static float k_d = 0.95f;

    inline static int initiated = 0;

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
};

#endif // SIMULATION_H
