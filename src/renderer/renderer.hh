#pragma once 

#include <SDL2/SDL.h>

#include "mesh.hh"

namespace renderer {
    void initialize(SDL_Window* window);

    void begin();

    void render(const mesh& mesh);

    void end();

    void destroy();
}