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
        for (int i = 0; i < this->layout.elements.size(); i++) {
            if (this->layout.elements[i].type == Types::GL_FLOAT_USER) {
                glVertexAttribPointer(
                    i, this->layout.elements[i].count, GL_FLOAT, GL_FALSE,
                    this->layout.m_stride * sizeof(float),
                    (void *)(size_t)this->layout.elements[i].offset);
            } else if (this->layout.elements[i].type == Types::GL_CHAR) {
                glVertexAttribPointer(
                    i, this->layout.elements[i].count, GL_BYTE, GL_FALSE,
                    this->layout.m_stride * sizeof(char),
                    (void *)(size_t)this->layout.elements[i].offset);
            } else if (this->layout.elements[i].type == Types::GL_INT_USER) {
                glVertexAttribPointer(
                    i, this->layout.elements[i].count, GL_BYTE, GL_FALSE,
                    this->layout.m_stride * sizeof(char),
                    (void *)(size_t)this->layout.elements[i].offset);
            }
            glEnableVertexAttribArray(i);
        }
    }
    void bind()
    {
        glBindVertexArray(m_id);
    }
};
