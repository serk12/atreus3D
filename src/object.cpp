#include <GL/glew.h>
#include <QTimer>
#include <iostream>
#include <fstream>

#include "object.h"

const char vertexVanillaPath[]   = "../atreus3D/shaders/vanilla.vert";
const char fragmentVanillaPath[] = "../atreus3D/shaders/vanilla.frag";

const char vertexSpherePath[]   = "../atreus3D/shaders/sphere.vert";
const char fragmentSpherePath[] = "../atreus3D/shaders/sphere.frag";
const char geometriSpherePath[] = "../atreus3D/shaders/sphere.geo";

namespace
{
    int LoadShader(const char *shaderSource, GLenum shaderType)
    {
        int shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);

        int success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
           glGetShaderInfoLog(shader, 1024, NULL, infoLog);
           std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << "\n" << shaderSource << std::endl;
           return -1;
        }
        return shader;
    }

    int LoadShaderFromFile(const char* shaderPath, GLenum shaderType)
    {
        std::ifstream shaderFile;

        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            shaderFile.open(shaderPath);
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            std::string shaderSource = shaderStream.str();
            return LoadShader(shaderSource.c_str(), shaderType);
        } catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ:" << shaderPath << std::endl;
            return -1;
        }
    }

    int LoadProgram(const char *vPath, const char *fPath, const char *gPath = nullptr)
    {
        int vertexShader = LoadShaderFromFile(vPath, GL_VERTEX_SHADER);
        int fragmentShader = LoadShaderFromFile(fPath, GL_FRAGMENT_SHADER);
        if (vertexShader < 0 or fragmentShader < 0) {
            return -1;
        }
        int geometryShader = -1;
        if (gPath != nullptr) {
            geometryShader = LoadShaderFromFile(gPath, GL_GEOMETRY_SHADER);
            if (geometryShader < 0) return -1;
        }

        int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        if(geometryShader >= 0) {
            glAttachShader(program, geometryShader);
        }
        glLinkProgram(program);
        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            return -1;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if (geometryShader >= 0) {
            glDeleteShader(geometryShader);
        }
        return program;
    }
}

Object::~Object()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Object::cameraMatrixCalc(data_visualization::Camera camera_)
{
        camera_.SetViewport();

        Object::projection = camera_.SetProjection();
        Object::view = camera_.SetView();
        Object::model = camera_.SetModel();
        Object::t = Object::view * Object::model;

        for (int i = 0; i < 3; ++i)
          for (int j = 0; j < 3; ++j)
              Object::normal(i, j) = Object::t(i, j);

        Object::normal = Object::normal.inverse().transpose();
    };

int Object::loadShader(const char * vertex, const char *frag, const char *geo = nullptr)
{
    return LoadProgram(vertex, frag, geo);
}

bool Object::vanillaProgramsLoad()
{
    int vanilla = LoadProgram(vertexVanillaPath, fragmentVanillaPath);
    int sphere = LoadProgram(vertexSpherePath, fragmentSpherePath, geometriSpherePath);
    if (vanilla <= 0 || sphere <= 0) return false;
    Object::programsList.resize(QTTYSHADERS);
    Object::programsList[int(ShaderType::Vanilla)] = vanilla;
    Object::programsList[int(ShaderType::Sphere)] = sphere;
    return true;
}

void Object::deleteVanillas()
{
    for (int vanilla : Object::programsList) {
        glDeleteProgram(vanilla);
    }
}

void Object::renderType(int type) const
{
    unsigned int program = programsList[int(shaderType)];
    glUseProgram(program);

    glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, Object::lightColor.data());
    glUniform3fv(glGetUniformLocation(program, "objectColor"), 1, objectColor.data());

    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, Object::view.data());    
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, Object::projection.data());

    Eigen::Affine3f translation;
    translation.matrix() = Object::model;
    translation.translation() = p;
    Eigen::Matrix4f modelObject = translation.matrix();
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, modelObject.data());

    switch (shaderType) {
    case ShaderType::Sphere:
        glUniform1f(glGetUniformLocation(program, "rSphere"), getRadius());
        break;
    default:
        break;
    }

    glBindVertexArray(VAO);
    glDrawElements(type, qttyFaces, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Object::load()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    if (indices.size() > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);
    }

    // set position to location = 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Object::solver(const float dtMs)
{
    float dt = dtMs/1000.0f;

    Eigen::Vector3f aux_p = p;
    Eigen::Vector3f aux_v = v;
    switch(solverType) {
    case SolverType::Euler:
        p = p + dt*v;
        v = v + dt*(w_i*f);
        break;
    case SolverType::SemiEuler:
        v = v + dt*(w_i*f);
        p = p + dt*v;
        break;
    case SolverType::Verlet:
        p = p + k_d*(p-p_pass)+dt*(dt*(f*w_i));
        v = (p - aux_p)/dt;
        break;
    case SolverType::RungeKuta2: //semiEuler for the moment
        v = v + dt*(w_i*f);
        p = p + dt*(v+v)/2.0f;
        break;
    default:
        break;
    }
    v_pass = aux_v;
    p_pass = aux_p;
}

void Object::initSolver()
{
    if (m == -1) physicsType = PhysicsType::Immovable;
    else if (m == -2) physicsType = PhysicsType::Transparent;
    w_i = (m < 0.0f)? 0.0f : 1.0f/m;
    f = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    p_pass = p - (v * 0.016f);
    v_pass = v;
}

void Object::update(const float deltatime, const std::list<Object*>& meshs)
{
    solver(deltatime);
    bool result = collisionDetect(meshs);
    if (result) {
        propagateCollision(meshs);
    }
}

void Object::correctObject(const Eigen::Vector3f& n, const float d, bool add)
{
    float d_aux = (this->getRadius()*n).norm();
    d_aux = d + (add? d_aux : -d_aux);
    Eigen::Vector3f vt = v - ((n.dot(v)) * n);
    v = (v - (1.0f + e) * (n.dot(v)) * n) - (u * vt);
    p = p - (1.0f + e) * (n.dot(p)+d_aux) * n;
    p_pass = p - (v * 0.016f);
}

float Object::getWeight() const
{
    return m;
}

float Object::getIWeight() const
{
    return w_i;
}

float Object::getElasticity() const
{
    return e;
}

float Object::getFriction() const
{
    return u;
}

Eigen::Vector3f Object::getPosition() const
{
    return p;
}

Eigen::Vector3f Object::getPassPosition() const
{
    return p_pass;
}

Eigen::Vector3f Object::getVelocity() const
{
    return v;
}

Eigen::Vector3f Object::getPassVelocity() const
{
    return v_pass;
}

void Object::setSolverModel(SolverType solverType)
{
    Object::solverType = solverType;
}

void Object::setGravityScale(const float scale)
{
    Object::gravityScale = scale;
    Object::gravity = Object::gravityScale * GENERAL_GRAVITY;
}

void Object::setKd(float kd)
{
    Object::k_d = kd;
}

