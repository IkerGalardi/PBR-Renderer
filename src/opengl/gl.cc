#include "gl.hh"

namespace gl {
    void set_clear_color(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void clear(uint32_t clear_mask) {
        glClear(clear_mask);
    }

    void draw(const gl::shader& shader, const gl::buffer& buffer) {
        // if there is any vertex array, unbind 
        glBindVertexArray(0);

        // Bind the shader program to be used
        glUseProgram(shader.id);
        
        // Bind and draw the specified buffer
        glBindBuffer(static_cast<uint32_t>(buffer.btype), buffer.id);
        glDrawBuffer(GL_BACK);
    }

    void draw(const gl::shader& shader, const gl::vertex_array& varray, uint32_t n) {
        // Bind the specified vertex array
        glBindVertexArray(varray.id);

        // Bind the shader program to be used
        glUseProgram(shader.id);

        glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
    }
}