#include "mesh.hh"

#include <spdlog/spdlog.h>

mesh::mesh(const std::vector<vertex>& vertices, const std::vector<unsigned int>& elements) {
    vertex_array = std::make_shared<GL::VertexArray>();
    vertex_array->Bind();
    spdlog::trace("Renderer: vertex array created");

    float test_vertices[] = {
        // positions          // random           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    vertex_buffer = std::make_shared<GL::Buffer>(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vertex_buffer->Bind();
    vertex_buffer->SetData(test_vertices, sizeof(test_vertices));
    spdlog::trace("Renderer: vertex buffer sent");

    element_buffer = std::make_shared<GL::Buffer>(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    element_buffer->Bind();
    element_buffer->SetData(indices, sizeof(indices));
    spdlog::trace("Renderer: element buffer sent");

    vertex_array->SetAttributes({
        {3, GL_FLOAT}, // Positions
        {3, GL_FLOAT}, // Normals
        {2, GL_FLOAT}  // Texture coordinates
    });
}