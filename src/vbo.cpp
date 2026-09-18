#include "vbo.h"

void VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

VBO::~VBO() { glDeleteBuffers(1, &m_id); }
