#include "shader.hh"

#include <fstream>
#include <iostream>

#include <GL/glew.h>

namespace gl
{
    shader::shader(const std::string &vs, const std::string &fs) {
        const char *vertex_source = vs.c_str();
        const char *fragment_source = fs.c_str();

        uint32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
        glCompileShader(vertex_shader);
        print_if_errors(vertex_shader);

        uint32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
        glCompileShader(fragment_shader);
        print_if_errors(fragment_shader);

        id = glCreateProgram();
        glAttachShader(id, vertex_shader);
        glAttachShader(id, fragment_shader);
        glLinkProgram(id);
        glValidateProgram(id);

        is_initialized = true;
    }

    shader::shader(const std::filesystem::path &vs, const std::filesystem::path &fs) {
        std::ifstream vertex_file(vs);
        std::stringstream vs_stream;
        vs_stream << vertex_file.rdbuf();

        std::ifstream fragment_file(vs);
        std::stringstream fs_stream;
        fs_stream << fragment_file.rdbuf();

        shader(vs_stream.str(), fs_stream.str());

        std::cout << "Shader destroyed" << std::endl;
    }

    shader::~shader() {
        if (is_initialized)
            glDeleteProgram(id);
    }

    void shader::set_int(const std::string& name, int value) {
        glUseProgram(id);
        int loc = glGetUniformLocation(id, name.c_str());
        glUniform1i(loc, value);
    }

    void shader::set_vector(const std::string &name, glm::vec2 vector) {
        glUseProgram(id);
        int loc = glGetUniformLocation(id, name.c_str());
        glUniform2f(loc, vector.x, vector.y);
    }

    void shader::set_vector(const std::string &name, glm::vec3 vector) {
        glUseProgram(id);
        int loc = glGetUniformLocation(id, name.c_str());
        glUniform3f(loc, vector.x, vector.y, vector.z);
    }

    void shader::set_vector(const std::string &name, glm::vec4 vector) {
        glUseProgram(id);
        int loc = glGetUniformLocation(id, name.c_str());
        glUniform4f(loc, vector.x, vector.y, vector.z, vector.w);
    }

    void shader::set_matrix(const std::string &name, glm::mat4 matrix) {
        glUseProgram(id);
        int loc = glGetUniformLocation(id, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);
    }

    void shader::print_if_errors(uint32 shader) {
        int32 succeded;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &succeded);

        if(!succeded) {
            char shader_log_info[512];
            int32 length;
            glGetShaderInfoLog(shader, 512 * sizeof(char), &length, shader_log_info);

            /// TODO: print through opengl logger
            std::cout << shader_log_info << std::endl;
        }
    }
}