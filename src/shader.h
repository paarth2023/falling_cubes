#pragma once

#include <filesystem>
#include <glm/glm.hpp>

class Shader {
    unsigned int m_id;

  public:
    Shader(std::filesystem::path &vertexShader,
           std::filesystem::path &fragmentShader);
    // copy constructor
    Shader(Shader &) = delete;
    // move constructor
    Shader(Shader &&) = delete;
    // copy assignment
    Shader &operator=(Shader &) = delete;
    // move assignment
    Shader &operator=(Shader &&) = delete;
    // destructor
    ~Shader();

    // member functions
    void use();
    void setInt(const std::string &location, int value);
    void setFloat(const std::string &location, float value);
    void setBool(const std::string &location, int value);
    void setMatrix(const std::string &location, glm::mat4 &mat);
};
