#include "mesh.hh"

#include <spdlog/spdlog.h>

mesh::mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& elements) {
    vertex_array = std::make_shared<GL::VertexArray>();
    vertex_array->Bind();
    spdlog::trace("Renderer: vertex array created");

    vertex_buffer = std::make_shared<GL::Buffer>(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vertex_buffer->Bind();
    vertex_buffer->SetData(vertices.data(), vertices.size() * sizeof(vertex));
    spdlog::trace("Renderer: vertex buffer sent");

    element_buffer = std::make_shared<GL::Buffer>(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    element_buffer->Bind();
    element_buffer->SetData(elements.data(), elements.size() * sizeof(unsigned int));
    spdlog::trace("Renderer: element buffer sent");

    vertex_array->SetAttributes({
        {3, GL_FLOAT}, // Positions
        {3, GL_FLOAT}, // Normals
        {2, GL_FLOAT}, // Texture coordinates
    });
}