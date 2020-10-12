#ifndef SIMULATION_H
#define SIMULATION_H

#include "object.h"
#include <list>

class Simulation {
public:
    static bool loadSim(std::pair<std::list<Object*>, std::list<Object*> >& objects);
    static void addParticle(std::list<Object*>& particleList);
};

#endif // SIMULATION_H
