#include "model.hh"

#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <spdlog/spdlog.h>
#include <objloader/OBJ_Loader.h>

model::model(const std::filesystem::path& mesh,
             const std::filesystem::path& diffuse,
             const std::filesystem::path& roughness,
             const std::filesystem::path& normal) {
    this->diffuse = std::make_shared<GL::Texture>(diffuse.c_str());
    this->roughness = std::make_shared<GL::Texture>(roughness.c_str());
    this->normal = std::make_shared<GL::Texture>(normal.c_str());
    spdlog::trace("Renderer: loaded textures {}, {}, {}", diffuse.c_str(), roughness.c_str(), normal.c_str());

    objl::Loader loader;
    bool load_status = loader.LoadFile(mesh.string());
    if(!load_status) {
        spdlog::error("Renderer: could not load the model {}", mesh.string());
        std::exit(1);
    }

    auto& vertices = loader.LoadedVertices;

    // Load the vertices and transform to engine format
    std::vector<vertex> real_vertices;
    real_vertices.reserve(vertices.size());
    std::transform(vertices.begin(), vertices.end(), vertices.begin(), [&real_vertices](objl::Vertex v) {
        /// TODO: maybe use emplace back instead of push back??
        real_vertices.push_back({{v.Position.X, v.Position.Y, v.Position.Z},
                                 {v.Normal.X, v.Normal.Y, v.Normal.Z},
                                 {v.TextureCoordinate.X, v.TextureCoordinate.Y}});

        return v;
    });

    /// TODO: merge with before loop, double iteration is stupid
    for(int i = 0; i < real_vertices.size(); i += 3) {
        vertex& v0 = real_vertices[i + 0];
        vertex& v1 = real_vertices[i + 1];
        vertex& v2 = real_vertices[i + 2];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;

        float delta_u1 = v1.texture_coordinates.x - v0.texture_coordinates.x;
        float delta_v1 = v1.texture_coordinates.y - v0.texture_coordinates.y;
        float delta_u2 = v2.texture_coordinates.x - v0.texture_coordinates.x;
        float delta_v2 = v2.texture_coordinates.y - v0.texture_coordinates.y;

        float f = delta_u1 * delta_v2 - delta_u2 * delta_v1;

        glm::vec3 tangent, bitangent;

        tangent.x = f * (delta_v2 * edge1.x - delta_v1 * edge2.x);
        tangent.y = f * (delta_v2 * edge1.y - delta_v1 * edge2.y);
        tangent.z = f * (delta_v2 * edge1.z - delta_v1 * edge2.z);

        bitangent.x = f * (-delta_u2 * edge1.x + delta_u1 * edge2.x);
        bitangent.y = f * (-delta_u2 * edge1.y + delta_u1 * edge2.y);
        bitangent.z = f * (-delta_u2 * edge1.z + delta_u1 * edge2.z);

        v0.tangent = tangent;
        v1.tangent = tangent;
        v2.tangent = tangent;
        v0.bitangent = bitangent;
        v1.bitangent = bitangent;
        v2.bitangent = bitangent;
    }

    mesh_data = std::make_shared<::mesh>(real_vertices, loader.LoadedIndices);
    vertex_count = real_vertices.size();
}