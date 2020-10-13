#ifndef SIMULATION_H
#define SIMULATION_H

#include "object.h"
#include <list>

class Simulation {
public:
    enum ScenaryType {Cascade = 0, Rain = 1, Fountain = 2, String = 3, Debug = 4};
    static bool loadSim(std::pair<std::list<Object*>, std::list<Object*> >& objects, ScenaryType type);
    static void addParticle(std::list<Object*>& particleList);

    inline static ScenaryType scenaryType = ScenaryType::Cascade;
    inline static Object::SolverType solverType = Object::SolverType::Verlet;
    inline static unsigned int liveTime = 1000;
    inline static unsigned int birdTime = 10;
    inline static unsigned int maxParticles = 10;
};

#endif // SIMULATION_H
