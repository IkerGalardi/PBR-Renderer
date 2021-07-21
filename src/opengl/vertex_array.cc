#include "vertex_array.hh"

static int size_from_gl_type(uint32 type)
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

namespace gl {
        vertex_array::vertex_array() {
            glGenVertexArrays(1, &id);
        }

        vertex_array::~vertex_array() {
            glDeleteVertexArrays(1, &id);
        }

        void vertex_array::add_buffer(const gl::buffer& buffer) {
            glBindVertexArray(id);
            glBindBuffer(static_cast<uint32>(buffer.btype), buffer.id);
        }

        void vertex_array::set_layout(std::initializer_list<attribute> attribs) {
            unsigned int stride = 0;            
            for(attribute attrib : attribs)
                stride += attrib.count * size_from_gl_type(static_cast<uint32>(attrib.type));
            
            glBindVertexArray(id);
            
            unsigned int i = 0;
            unsigned long offset = 0;
            for(attribute attrib : attribs) {
                glEnableVertexAttribArray(i);
                int size = size_from_gl_type(static_cast<uint32>(attrib.type));
                glVertexAttribPointer(i, attrib.count, static_cast<uint32>(attrib.type), GL_FALSE, stride, (const void*)offset);
                offset += attrib.count * size;
                i++;
            }
        }
}