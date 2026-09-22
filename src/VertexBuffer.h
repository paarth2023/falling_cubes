#pragma once

#include <glad/glad.h>

class VertexBuffer {
    unsigned int m_id;

  public:
    VertexBuffer()
    {
        glGenBuffers(1, &m_id);
    }
    VertexBuffer(float *vertices, unsigned int vertexCount)
    {
        glGenBuffers(1, &m_id);
        bind();
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices,
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
