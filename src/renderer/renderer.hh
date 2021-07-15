#pragma once 

#include <SDL2/SDL.h>

namespace renderer {
    void initialize(SDL_Window* window);

    void render();

    void destroy();
}