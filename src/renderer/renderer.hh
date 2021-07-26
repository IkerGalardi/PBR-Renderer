#pragma once 

#include <SDL2/SDL.h>

#include "model.hh"
#include "mesh.hh"

namespace renderer {
    void initialize(SDL_Window* window);

    void begin();

    void render(const model& mesh);

    void end();

    void destroy();
}