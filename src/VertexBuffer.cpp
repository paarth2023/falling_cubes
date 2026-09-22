#include "VertexBuffer.h"

void VertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

VertexBuffer::~VertexBuffer()
{
    if (m_id != 0)
        glDeleteBuffers(1, &m_id);
}
VertexBuffer::VertexBuffer(VertexBuffer &&other) : m_id(other.m_id)
{
    other.m_id = 0;
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other)
{
    if (this != &other) {
        m_id = other.m_id;
        other.m_id = 0;
    }
    return *this;
}
