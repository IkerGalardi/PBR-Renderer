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
    auto* window = platform::get().create_window(2000, 2000, "OpenGL engine");

    spdlog::set_level(spdlog::level::info);

    renderer::initialize(window);

    renderer::scene_data scene_data;
    scene_data.camera_position = {0.0f, 0.0f, 3.0f};
    scene_data.camera_fov = 60.0f;

    model m(std::filesystem::path{"models/sphere.obj"},
            std::filesystem::path{"models/suzanne/suzanne_color.png"}, 
            std::filesystem::path{"models/suzanne/suzanne_roughness.png"},
            std::filesystem::path{"models/suzanne/suzanne_normal.png"});
    m.position = {0.0f, 0.0f, 0.0f};

    platform::get().loop([&](SDL_Event& event) {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                spdlog::info("SDL: quit event arrived");
                return true;
            }
        }

        renderer::begin(scene_data);

        renderer::render(m);

        renderer::end();

        return false;
    });

    // Cleanup
}