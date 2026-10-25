#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static std::string readFile(const std::filesystem::path &path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

Shader::Shader(const std::filesystem::path &vertexShader,
               const std::filesystem::path &fragmentShader)
{
    std::string vertexCode = readFile(vertexShader);
    std::string fragmentCode = readFile(fragmentShader);

    const char *vertexShaderSource = vertexCode.c_str();
    const char *fragmentShaderSource = fragmentCode.c_str();

    int success;
    char infoLog[512];

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, sizeof(infoLog), NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, sizeof(infoLog), NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, sizeof(infoLog), NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

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

void Shader::setMatrix(const std::string &location, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, location.c_str()), 1,
                       GL_FALSE, glm::value_ptr(mat));
}
