#include "mesh.hh"

#include <spdlog/spdlog.h>

mesh::mesh(std::vector<vertex> vertices, std::vector<unsigned int> elements) {
    vertex_array = std::make_shared<GL::VertexArray>();
    vertex_array->Bind();

    vertex_array->SetAttributes({
        {3, GL_FLOAT}, // Positions
        {3, GL_FLOAT}, // Normals
        {2, GL_FLOAT}  // Texture coordinates
    });

    vertex_buffer->Bind();
    vertex_buffer->SetData(vertices.data(), vertices.size() * sizeof(vertex));

    element_buffer->Bind();
    element_buffer->SetData(elements.data(), elements.size() * sizeof(unsigned int));
}