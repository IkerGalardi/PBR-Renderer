#pragma once

#include "mesh.hh"

struct model {
    model(const std::filesystem::path& mesh,
          const std::filesystem::path& diffuse,
          const std::filesystem::path& roughness,
          const std::filesystem::path& normal);

    std::shared_ptr<mesh> mesh_data;
    std::shared_ptr<GL::Texture> diffuse;
    std::shared_ptr<GL::Texture> roughness;
    std::shared_ptr<GL::Texture> normal;
};