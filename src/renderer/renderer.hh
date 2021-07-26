#pragma once 

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "model.hh"
#include "mesh.hh"

namespace renderer {
    struct scene_data {
        glm::vec3 camera_position;
        float camera_fov;
    };

    void initialize(SDL_Window* window);

    void begin(const scene_data& scene_data);

    void render(const model& mesh);

    void end();

    void destroy();
}