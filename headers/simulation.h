#ifndef SIMULATION_H
#define SIMULATION_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Simulation; }
QT_END_NAMESPACE

#include "object.h"
#include <list>

class Simulation : public QMainWindow
{
    Q_OBJECT
public:
    Simulation(QWidget *parent = nullptr);
    ~Simulation();

    enum ScenaryType {Cascade, Rain, Fountain, String, Debug, DebugS};
    static bool loadSim(std::pair<std::list<Object*>, std::list<Object*> >& objects, ScenaryType type);
    static void addParticle(std::list<Object*>& particleList);

    inline static ScenaryType scenaryType = ScenaryType::String;
    inline static Object::SolverType solverType = Object::SolverType::Verlet;

    inline static unsigned int liveTime = 10000000;
    inline static unsigned int birdTime = 100;
    inline static unsigned int maxParticles = 500;

private:
    Ui::Simulation *ui;

private slots:
    void on_GravityScale_valueChanged(int value);
    void on_TopParticles_valueChanged(int value);
    void on_BirdTime_valueChanged(int value);
    void on_LiveTime_valueChanged(int value);
};

#endif // SIMULATION_H
