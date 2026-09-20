#pragma once

#include <vector>

struct BufferElement {
    unsigned int count;
    unsigned int offset;
    unsigned int size;
};

class BufferLayout {
    unsigned int m_stride;
    std::vector<BufferElement> elements;

  public:
    BufferLayout();
    template <typename T> void push(unsigned int count);
};
