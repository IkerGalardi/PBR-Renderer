#pragma once

#include <filesystem>
#include <vector>

#include <glm/glm.hpp>

#include "opengl/OpenGL.hh"

struct vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;
};

struct mesh {
    mesh() = default;
    mesh(std::vector<vertex> vertices, std::vector<unsigned int> elements);

    std::shared_ptr<GL::VertexArray> vertex_array;
    std::shared_ptr<GL::Buffer> vertex_buffer;
    std::shared_ptr<GL::Buffer> element_buffer;
};