#pragma once

#include <vector>
#include <typeinfo>

enum class Types { GL_FLOAT_USER, GL_INT_USER, GL_CHAR };

struct BufferElement {
    unsigned int count;
    unsigned int offset;
    unsigned long size;
    Types type;
};

class BufferLayout {
  public:
    unsigned int m_stride;
    std::vector<BufferElement> elements;
    BufferLayout();
    template <typename T> void push(unsigned int count)
    {
        Types type;
        if (typeid(T) == typeid(int))
            type = Types::GL_INT_USER;
        else if (typeid(T) == typeid(float))
            type = Types::GL_FLOAT_USER;
        else if (typeid(T) == typeid(char))
            type = Types::GL_CHAR;
        if (!elements.size()) {
            BufferElement obj = {count, 0, count * sizeof(T), type};
            elements.push_back(obj);
        } else {
            BufferElement new_obj = {count, m_stride, count * sizeof(T), type};
            elements.push_back(new_obj);
        }
        m_stride += count * sizeof(T);
    }

    std::vector<BufferElement> &getElements()
    {
        return elements;
    }
};
