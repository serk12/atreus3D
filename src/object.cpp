#include <GL/glew.h>
#include <iostream>
#include <fstream>

#include "object.h"

Eigen::Matrix4f Object::projection, Object::view, Object::model, Object::t, Object::normal;
unsigned int Object::vanilla;

const char vertexShaderPath[] = "../atreus3D/shaders/vanilla.vert";
const char fragmentShaderPath[] = "../atreus3D/shaders/vanilla.frag";

namespace {

    int LoadShader(const char *shaderSource, GLenum shaderType) {
        int shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);

        int success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
           glGetShaderInfoLog(shader, 1024, NULL, infoLog);
           std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
           return -1;
        }
        return shader;
    }

    int LoadShaderFromFile(const char* shaderPath, GLenum shaderType) {
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
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            return -1;
        }
    }

    int LoadProgram(const char *vPath, const char *fPath, const char *gPath = nullptr) {
        int vertexShader = LoadShaderFromFile(vPath, GL_VERTEX_SHADER);
        int fragmentShader = LoadShaderFromFile(fPath, GL_FRAGMENT_SHADER);
        if (vertexShader < 0 or fragmentShader < 0) {
            return -1;
        }
        int geometryShader = -1;
        if (gPath != nullptr) {
            geometryShader = LoadShader(gPath, GL_GEOMETRY_SHADER);
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

void Object::cameraMatrixCalc(data_visualization::Camera camera_) {
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

bool Object::vanillaProgramLoad()
{
    int shader = LoadProgram(vertexShaderPath, fragmentShaderPath);
    Object::vanilla = shader;
    return shader >= 0;
}

void Object::render() const
{
    glUseProgram(program);

    Eigen::Vector3f lightColor(1.0f, 1.0f, 1.0f);
    glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, lightColor.data());
    Eigen::Vector3f objectColor(0.7f, 0.5f, 0.2f);

    glUniform3fv(glGetUniformLocation(program, "objectColor"), 1, objectColor.data());

    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, Object::view.data());
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, Object::model.data());
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, Object::projection.data());

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, qttyFaces, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


