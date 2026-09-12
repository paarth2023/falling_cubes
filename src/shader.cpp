#include "shader.h"
#include <fstream>

Shader::Shader() {}

Shader::Shader(std::string &vertexPath, std::string &fragmentPath)
{
    std::ifstream vertexFile(vertexPath);
    vertexFile.seekg(0, vertexFile.end);
    int vertexSize = vertexFile.tellg();
    vertexFile.seekg(0, vertexFile.beg);

    vertexShaderSource = new char[vertexSize];
    vertexFile.read(vertexShaderSource, vertexSize);

    std::ifstream fragmentFile(fragmentPath);
    fragmentFile.seekg(0, fragmentFile.end);
    int fragmentSize = fragmentFile.tellg();
    fragmentFile.seekg(0, fragmentFile.beg);

    fragmentShaderSource = new char[fragmentSize];
    fragmentFile.read(fragmentShaderSource, fragmentSize);
}

void Shader::setVertexSource(std::string &vertexPath)
{
    std::ifstream vertexFile(vertexPath);
    vertexFile.seekg(0, vertexFile.end);
    int vertexSize = vertexFile.tellg();
    vertexFile.seekg(0, vertexFile.beg);

    vertexShaderSource = new char[vertexSize];
    vertexFile.read(vertexShaderSource, vertexSize);
}

void Shader::setFragmentSource(std::string &fragmentPath)
{
    std::ifstream fragmentFile(fragmentPath);
    fragmentFile.seekg(0, fragmentFile.end);
    int fragmentSize = fragmentFile.tellg();
    fragmentFile.seekg(0, fragmentFile.beg);

    fragmentShaderSource = new char[fragmentSize];
    fragmentFile.read(fragmentShaderSource, fragmentSize);
}

void Shader::compileVertexShader()
{
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderID);
}

void Shader::compileFragmentShader()
{
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderID);
}

void Shader::linkShaderProgram()
{
    ID = glCreateProgram();
    glAttachShader(ID, vertexShaderID);
    glAttachShader(ID, fragmentShaderID);
    glLinkProgram(ID);
}

void Shader::use() { glUseProgram(ID); }

void Shader::setInt(const GLchar *target, int val)
{
    GLint location = glGetUniformLocation(ID, target);
    glUniform1i(location, val);
}

void Shader::setFloat(const GLchar *target, float val)
{
    GLint location = glGetUniformLocation(ID, target);
    glUniform1f(location, val);
}

void Shader::setBool(const GLchar *target, bool val)
{
    GLint location = glGetUniformLocation(ID, target);
    glUniform1i(location, val);
}

void Shader::setMatrix4fv(const GLchar *target, GLfloat *val)
{
    GLint location = glGetUniformLocation(ID, target);
    glUniformMatrix4fv(location, 1, GL_FALSE, val);
}

Shader::~Shader()
{
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    glDeleteProgram(ID);
}
