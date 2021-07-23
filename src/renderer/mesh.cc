#include "mesh.hh"

mesh::mesh(const std::filesystem::path& obj, const std::filesystem::path& texture) {
    vertex_array = std::make_shared<GL::VertexArray>();
    vertex_array->Bind();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    };

    unsigned int elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    vertex_buffer = std::make_shared<GL::Buffer>(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    vertex_buffer->Bind();
    vertex_buffer->SetData(vertices, sizeof(vertices));

    element_buffer = std::make_shared<GL::Buffer>(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    element_buffer->Bind();
    element_buffer->SetData(elements, sizeof(elements));

    vertex_array->SetAttributes({
        {3, GL_FLOAT},
        {2, GL_FLOAT}
    });
}