#ifndef SIMULATION_H
#define SIMULATION_H

#include "object.h"

class Simulation {
public:
    static bool loadSim(std::vector<Object*>& objects);
};

#endif // SIMULATION_H
