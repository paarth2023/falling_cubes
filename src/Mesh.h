#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "buffer_layout.h"

class Mesh {
private:
    VertexArray m_vao;
    unsigned int m_vertexCount;

public:
    Mesh(const float *vertices, unsigned int floatCount, unsigned int vertexCount, BufferLayout layout);

    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;
    Mesh(Mesh &&) noexcept = default;
    Mesh &operator=(Mesh &&) noexcept = default;
    ~Mesh() = default;

    void draw() const;

    static Mesh createCube();
};
