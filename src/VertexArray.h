#pragma once
#include <glad/glad.h>
#include "VertexBuffer.h"
#include "./buffer_layout.h"

class VertexArray {
    unsigned int m_id;
    VertexBuffer vbo;
    BufferLayout layout;

  public:
    VertexArray(VertexBuffer vbo, BufferLayout layout)
    {
        this->vbo = std::move(vbo);
        this->layout = std::move(layout);
        glGenVertexArrays(1, &m_id);
        bind();
        this->vbo.bind();
        auto elements = this->layout.getElements();
        for (int i = 0; i < elements.size(); i++) {
            if (elements[i].type == Types::GL_FLOAT_USER) {
                glVertexAttribPointer(i, elements[i].count, GL_FLOAT, GL_FALSE,
                                      this->layout.m_stride,
                                      (void *)(size_t)elements[i].offset);
            } else if (elements[i].type == Types::GL_CHAR) {
                glVertexAttribPointer(i, elements[i].count, GL_BYTE, GL_FALSE,
                                      this->layout.m_stride,
                                      (void *)(size_t)elements[i].offset);
            } else if (elements[i].type == Types::GL_INT_USER) {
                glVertexAttribPointer(i, elements[i].count, GL_BYTE, GL_FALSE,
                                      this->layout.m_stride,
                                      (void *)(size_t)elements[i].offset);
            }
            glEnableVertexAttribArray(i);
        }
    }
    void bind()
    {
        glBindVertexArray(m_id);
    }
};
