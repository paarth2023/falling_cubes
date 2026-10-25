#pragma once
#include <glad/glad.h>
#include "VertexBuffer.h"
#include "./buffer_layout.h"

class VertexArray {
    unsigned int m_id;
    VertexBuffer vbo;
    BufferLayout layout;

  public:
    VertexArray() : m_id(0) {}
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

    ~VertexArray()
    {
        if (m_id != 0) {
            glDeleteVertexArrays(1, &m_id);
        }
    }

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    VertexArray(VertexArray &&other) noexcept
        : m_id(other.m_id), vbo(std::move(other.vbo)), layout(std::move(other.layout))
    {
        other.m_id = 0;
    }

    VertexArray &operator=(VertexArray &&other) noexcept
    {
        if (this != &other) {
            if (m_id != 0) {
                glDeleteVertexArrays(1, &m_id);
            }
            m_id = other.m_id;
            vbo = std::move(other.vbo);
            layout = std::move(other.layout);
            other.m_id = 0;
        }
        return *this;
    }

    void bind() const
    {
        glBindVertexArray(m_id);
    }
};
