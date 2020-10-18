#ifndef UTILS_H
#define UTILS_H

#define GENERAL_E 0.95f
#define GENERAL_U 0.95f
#define GENERAL_M 0.05f
#define GENERAL_R 0.0175f
#define GENERAL_D 0.01f
#define GENERAL_RM 0.25f
#define GENERAL_K_D 0.95f
#define GENERAL_K_ELAS 10.05f
#define GENERAL_K_DUMP 2.5f
#define GENERAL_LIVE_TIME 100000
#define GENERAL_BIRTH_TIME 100
#define GENERAL_MAX_PARTICLES 500
#define GENERAL_GRAVITY_SCALE 1.0f


#define GENERAL_SCENARY_TYPE Simulation::ScenaryType::String
#define GENERAL_SOLVER_TYPE Object::SolverType::Verlet

#define GENERAL_LIGHT Eigen::Vector3f(1.0f, 1.0f, 1.0f)
#define GENERAL_GRAVITY Eigen::Vector3f(0.0f, -9.81f, 0.0f)

#endif // UTILS_H
