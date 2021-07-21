#pragma once 

#include <GL/glew.h>

#include "common.hh"

/// TODO: 
///   - deep copy instead of shallow copy as it is right now

namespace gl {
    class shader;

    enum class buffer_type {
        array_buffer = GL_ARRAY_BUFFER,
        element_buffer = GL_ELEMENT_ARRAY_BUFFER
    };

    enum class draw_type {
        static_draw = GL_STATIC_DRAW,
        dynamic_draw = GL_DYNAMIC_DRAW
    };

    class buffer {
        friend void draw(const gl::shader& shader, const gl::buffer& buffer);
        friend class vertex_array;
    public:
        buffer(buffer_type btype, draw_type dtype);
        ~buffer();

        void set_data(const void* data, uint32 size);
    private:
        buffer_type btype;
        draw_type dtype;

        uint32 id;
    protected:
    };
}