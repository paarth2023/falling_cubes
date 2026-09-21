#pragma once

#include <glad/glad.h>

class VertexBuffer {
    unsigned int m_id;

  public:
    VertexBuffer() { glGenBuffers(1, &m_id); }
    void upload(const float *vertices);
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
