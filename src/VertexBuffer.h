#pragma once

#include <glad/glad.h>

class VertexBuffer {
    unsigned int m_id;

  public:
    VertexBuffer()
    {
        glGenBuffers(1, &m_id);
    }
    VertexBuffer(float *vertices)
    {
        glGenBuffers(1, &m_id);
        bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);
    }
    void bind();
    // Destructor
    ~VertexBuffer();
    // copy constructor
    VertexBuffer(const VertexBuffer &) = delete;
    // move constructor
    VertexBuffer(VertexBuffer &&other);
    // move assignment operator
    VertexBuffer &operator=(VertexBuffer &&other);
    // copy assignment operator
    VertexBuffer &operator=(const VertexBuffer &) = delete;
};
