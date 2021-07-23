#include "VertexArray.hh"

#include <GL/glew.h>

static int GetSizeFromGLType(unsigned int type)
{
    switch (type)
    {
    case GL_BYTE:               return sizeof(char);
    case GL_UNSIGNED_BYTE:      return sizeof(unsigned char);
    case GL_SHORT:              return sizeof(short);
    case GL_UNSIGNED_SHORT:     return sizeof(unsigned short);
    case GL_INT:                return sizeof(int);
    case GL_UNSIGNED_INT:       return sizeof(unsigned int);
    case GL_HALF_FLOAT:         return (sizeof(float) / 2);
    case GL_FLOAT:              return sizeof(float);
    case GL_DOUBLE:             return sizeof(double);
    default:                    return -1;
    }
}

namespace GL
{
        VertexArray::VertexArray()
        {
            glGenVertexArrays(1, &VertexArrayID);
        }

        VertexArray::~VertexArray()
        {
            glDeleteVertexArrays(1, &VertexArrayID);
        }


        void VertexArray::Bind()
        {
            glBindVertexArray(VertexArrayID);
        }
        void VertexArray::Unbind()
        {
            glBindVertexArray(0);
        }

        void VertexArray::SetAttributes(std::initializer_list<Attribute> attributes)
        {
            unsigned int stride = 0;            
            for(Attribute attrib : attributes)
                stride += attrib.Count * GetSizeFromGLType(attrib.Type);
            
            glBindVertexArray(VertexArrayID);
            
            unsigned int i = 0;
            unsigned long offset = 0;
            for(Attribute attrib : attributes) {
                glEnableVertexAttribArray(i);
                int size = GetSizeFromGLType(attrib.Type);
                glVertexAttribPointer(i, attrib.Count, attrib.Type, GL_FALSE, stride, (const void*)offset);
                offset += attrib.Count * size;
                i++;
            }
        }
}