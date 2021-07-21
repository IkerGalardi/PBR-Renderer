#pragma once

#include <initializer_list>

#include <GL/glew.h>

#include "buffer.hh"
#include "shader.hh"

namespace gl {
    enum class attrib_type {
        sbyte = GL_BYTE,
        ubyte = GL_UNSIGNED_BYTE,
        sshort = GL_SHORT,
        ushort = GL_UNSIGNED_SHORT,
        sint = GL_INT,
        uint = GL_UNSIGNED_INT,
        float16 = GL_HALF_FLOAT,
        float32 = GL_FLOAT,
        float64 = GL_DOUBLE
    };

    struct attribute {
        uint32 count;
        attrib_type type;
    };

    class vertex_array {
        friend void draw(const gl::shader& shader, const gl::vertex_array& varray, uint32 n);
    public:
        vertex_array();
        ~vertex_array();

        void add_buffer(const gl::buffer& buffer);
        void set_layout(std::initializer_list<attribute> attribs);
    private:
        uint32 id;
    protected:
    };
}