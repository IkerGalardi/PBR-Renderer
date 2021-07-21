#pragma once 

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "common.hh"
#include "buffer.hh"
#include "shader.hh"
#include "vertex_array.hh"

namespace gl {
    typedef void(error_callback)();

    void set_clear_color(const glm::vec4& color);
    void clear(uint32 clear_mask);

    void draw(const gl::shader& shader, const gl::buffer& buffer);
    void draw(const gl::shader& shader, const gl::vertex_array& varray, uint32 n);

    void debugging_information(bool enabled);
}