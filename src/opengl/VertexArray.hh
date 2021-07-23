#pragma once

#include <vector>
#include <initializer_list>

#include "Buffer.hh"

namespace GL
{
    struct Attribute
    {
        unsigned int Count;
        unsigned int Type;
    };

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind();
        void Unbind();

        void SetAttributes(std::initializer_list<Attribute> attributes); 
    private:
        unsigned int VertexArrayID;
    protected:
    };
}