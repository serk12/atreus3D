#include "simulation.h"

#include "particle.h"
#include "mesh.h"

bool Simulation::loadSim(std::vector<Object *> &objects)
{
    Mesh *a = new Mesh();
    objects.push_back(a);
    Particle *b = new Particle();
    objects.push_back(b);
    return true;
}
