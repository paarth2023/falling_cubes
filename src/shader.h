#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Shader {
    char *vertexShaderSource;
    char *fragmentShaderSource;
    GLuint ID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

  public:
    Shader();
    Shader(std::string &vertexPath, std::string &fragmentPath);
    void setVertexSource(std::string &vertexPath);
    void setFragmentSource(std::string &fragmentPath);
    void compileVertexShader();
    void compileFragmentShader();
    void linkShaderProgram();
    void use();
    void setInt(const GLchar *target, int val);
    void setFloat(const GLchar *target, float val);
    void setBool(const GLchar *target, bool val);
    void setMatrix4fv(const GLchar *target, GLfloat *val);
    ~Shader();
};
