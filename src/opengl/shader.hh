#pragma once 

#include <string>
#include <filesystem>

#include "glm/glm.hpp"

/// TODO: 
///    - Uniform location cacheing????

namespace gl {
    class buffer;
    class vertex_array;

    class shader {
        friend void draw(const gl::shader& shader, const gl::buffer& buffer);
        friend void draw(const gl::shader& shader, const gl::vertex_array& varray, uint32_t n);
    public:
        shader() = default;
        shader(const std::string& vs, const std::string& fs);
        shader(const std::filesystem::path& vs, const std::filesystem::path& fs);
        ~shader();

        void set_int(const std::string& name, int value);
        void set_vector(const std::string& name, glm::vec2 vector);
        void set_vector(const std::string& name, glm::vec3 vector);
        void set_vector(const std::string& name, glm::vec4 vector);
        void set_matrix(const std::string& name, glm::mat4 matrix);
    private:
        bool is_initialized = false;

        uint32_t id;

        void print_if_errors(uint32_t shader);
    protected:
    };
}