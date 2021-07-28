#include <iostream>
#include <vector>
#include <math.h>

#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>

#include "platform.hh"
#include "renderer/renderer.hh"
#include "renderer/mesh.hh"
#include "renderer/model.hh"

int main(int argc, char** argv) {
    auto* window = platform::get().create_window(1000, 1000, "test");

    renderer::initialize(window);

    renderer::scene_data scene_data;
    scene_data.camera_position = {0.0f, 0.0f, 0.0f};
    scene_data.camera_fov = 90.0f;

    model m(std::filesystem::path{"models/suzanne.obj"},
            std::filesystem::path{}, 
            std::filesystem::path{},
            std::filesystem::path{});
    m.position = {0.0f, 0.0f, 0.0f};

    platform::get().loop([&](SDL_Event& event) {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                spdlog::info("SDL: quit event arrived");
                return true;
            }
        }

        // Change position
        static double time = 0.0f;
        time += .001f;
        scene_data.camera_position.z = 2.0 * std::sin(time) + 2.5;
        spdlog::trace("{}", scene_data.camera_position.z);

        renderer::begin(scene_data);

        renderer::render(m);

        renderer::end();

        return false;
    });

    // Cleanup
}