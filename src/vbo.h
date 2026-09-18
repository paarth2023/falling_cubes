#pragma once

#include <glad/glad.h>

class VBO {
    unsigned int m_id;
    float *vertices;

  public:
    VBO(float *vertices)
    {
        glGenBuffers(1, &m_id);
        this->vertices = vertices;
    }
    void bind();
    // Destructor
    ~VBO();
    // copy constructor
    VBO(const VBO &) = delete;
    // move constructor
    VBO(const VBO &&) = delete;
    // copy assignment operator
    VBO &operator=(VBO &) = delete;
    // move assignment operator
    VBO &operator=(VBO &&) = delete;
};
