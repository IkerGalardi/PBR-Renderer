#pragma once

#include <filesystem>

#include "opengl/OpenGL.hh"

struct mesh {
    mesh() = default;
    mesh(const std::filesystem::path& obj, const std::filesystem::path& texture);

    std::shared_ptr<GL::VertexArray> vertex_array;
    std::shared_ptr<GL::Buffer> vertex_buffer;
    std::shared_ptr<GL::Buffer> element_buffer;
};