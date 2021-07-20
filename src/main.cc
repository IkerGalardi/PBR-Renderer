#include <iostream>
#include <vector>

#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>

#include "platform.hh"
#include "renderer/renderer.hh"

int main(int argc, char** argv) {
    auto* window = platform::get().create_window(1000, 1000, "test");

    renderer::initialize(window);

    platform::get().loop([&](SDL_Event& event) {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                spdlog::info("SDL: quit event arrived");
                return true;
            }
        }

        renderer::render();

        return false;
    });

    // Cleanup
}