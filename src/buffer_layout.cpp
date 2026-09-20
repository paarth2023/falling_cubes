#include "buffer_layout.h"

BufferLayout::BufferLayout() { m_stride = 0; }

template <typename T> void BufferLayout::push(unsigned int count)
{
    if (!elements.size()) {
        BufferElement obj = {count, 0, count * sizeof(T)};
        elements.push_back(obj);
    } else {
        BufferElement obj = elements[elements.size() - 1];
        BufferElement new_obj = {count, m_stride, count * sizeof(T)};
        elements.push_back(new_obj);
    }
    m_stride += count * sizeof(T);
}
