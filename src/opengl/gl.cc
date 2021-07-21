#include "gl.hh"

static void GLAPIENTRY message_callback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{

    std::string severityString = "HEHE";
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            severityString = "HIGH";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            severityString = "MEDIUM";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            severityString = "LOW";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION: return;
        
        default:
            severityString = "lel";
            break;
    }

    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = %s, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severityString.c_str(), message );

    if(severity == GL_DEBUG_SEVERITY_HIGH)
        std::exit(-1);
}

namespace gl {
    void set_clear_color(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void clear(uint32 clear_mask) {
        glClear(clear_mask);
    }

    void draw(const gl::shader& shader, const gl::buffer& buffer) {
        // if there is any vertex array, unbind 
        glBindVertexArray(0);

        // Bind the shader program to be used
        glUseProgram(shader.id);
        
        // Bind and draw the specified buffer
        glBindBuffer(static_cast<uint32>(buffer.btype), buffer.id);
        glDrawBuffer(GL_BACK);
    }

    void draw(const gl::shader& shader, const gl::vertex_array& varray, uint32 n) {
        // Bind the specified vertex array
        glBindVertexArray(varray.id);

        // Bind the shader program to be used
        glUseProgram(shader.id);

        glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
    }


    void debugging_information(bool enabled) {
        if(enabled) {
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(message_callback, nullptr);
        } else {
            glDisable(GL_DEBUG_OUTPUT);
        }
    }
}