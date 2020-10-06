#ifndef SIMULATION_H
#define SIMULATION_H

#include "object.h"
#include <list>

class Simulation {
public:
    static bool loadSim(std::pair<std::list<Object*>, std::list<Object*> >& objects);
};

#endif // SIMULATION_H
