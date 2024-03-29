#ifndef UTILS_H
#define UTILS_H

#define GENERAL_E 0.5f  // bouncing
#define GENERAL_U 0.2f  // friction
#define GENERAL_M 0.02f // Mass
#define GENERAL_R 0.0175f// radius
#define GENERAL_D 0.1f   // spring distance
#define GENERAL_RM 1.0f  // sphere radius
#define GENERAL_K_D 0.975f // verlet
#define GENERAL_K_ELAS 1.2f // spring elasticity
#define GENERAL_K_DUMP 0.15f // spring dumping
#define GENERAL_LIVE_TIME 5000
#define GENERAL_BIRTH_TIME 100
#define GENERAL_MAX_PARTICLES 500
#define GENERAL_GRAVITY_SCALE 0.2f


#define GENERAL_SCENARY_TYPE Simulation::ScenaryType::RigidBody
#define GENERAL_SOLVER_TYPE Object::SolverType::Verlet

#define GENERAL_LIGHT Eigen::Vector3f(1.0f, 1.0f, 1.0f)
#define GENERAL_GRAVITY Eigen::Vector3f(0.0f, -9.81f, 0.0f)

#endif // UTILS_H
