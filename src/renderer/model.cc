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
    // this->diffuse = std::make_shared<GL::Texture>(diffuse.c_str());
    // this->roughness = std::make_shared<GL::Texture>(roughness.c_str());
    // this->normal = std::make_shared<GL::Texture>(normal.c_str());

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

    mesh_data = std::make_shared<::mesh>(real_vertices, loader.LoadedIndices);
}