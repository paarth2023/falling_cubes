#pragma once

#include "VertexBuffer.h"
#include "buffer_layout.h"

class VertexArray {
    unsigned int m_id;
    BufferLayout layout;
    VertexBuffer vbo;

  public:
    VertexArray(VertexBuffer &vbo, BufferLayout &layout)
    {
        this->vbo = std::move(vbo);
        this->layout = std::move(layout);
        glGenVertexArrays(1, &m_id);
    }
    void init();
    void bind();
};
