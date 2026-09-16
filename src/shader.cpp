#include "shader.h"
#include <fstream>
#include <iostream>

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(std::filesystem::path &vertexShader,
               std::filesystem::path &fragmentShader)
{
    std::cout << vertexShader.string();
    std::ifstream vertexFile(vertexShader.string());
    vertexFile.seekg(0, vertexFile.end);
    int vertexSize = vertexFile.tellg();
    vertexFile.seekg(0, vertexFile.beg);
    char *vertexShaderSource = new char[vertexSize + 1];
    vertexFile.read(vertexShaderSource, vertexSize);
    vertexShaderSource[vertexSize] = 0;

    std::ifstream fragmentFile(fragmentShader.string());
    fragmentFile.seekg(0, fragmentFile.end);
    int fragmentSize = fragmentFile.tellg();
    fragmentFile.seekg(0, fragmentFile.beg);
    char *fragmentShaderSource = new char[fragmentSize + 1];
    fragmentFile.read(fragmentShaderSource, fragmentSize);
    fragmentShaderSource[fragmentSize] = 0;

    unsigned int vertex;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex);
    delete[] vertexShaderSource;

    unsigned int fragment;
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment);
    delete[] fragmentShaderSource;

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() { glDeleteProgram(m_id); }

void Shader::use() { glUseProgram(m_id); }

void Shader::setInt(const std::string &location, int value)
{
    glUniform1i(glGetUniformLocation(m_id, location.c_str()), value);
}

void Shader::setFloat(const std::string &location, float value)
{
    glUniform1f(glGetUniformLocation(m_id, location.c_str()), value);
}

void Shader::setBool(const std::string &location, int value)
{
    glUniform1f(glGetUniformLocation(m_id, location.c_str()), value);
}

void Shader::setMatrix(const std::string &location, glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, location.c_str()), 1,
                       GL_FALSE, glm::value_ptr(mat));
}
