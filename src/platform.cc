#include "platform.hh"

#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

platform::platform() {
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%l] %v%$");

    spdlog::info("Logging prepared.");

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        /// TODO: show detailed error message
        spdlog::error("SDL: could not be initialized");
        std::exit(1);
    }
    spdlog::info("SDL: initialized");
}

platform::~platform() {
    SDL_DestroyWindow(window);
}


SDL_Window* platform::create_window(unsigned int width, unsigned int height, const std::string& title) {
    this->window = SDL_CreateWindow("Vulkan Engine",
		                            SDL_WINDOWPOS_UNDEFINED,
		                            SDL_WINDOWPOS_UNDEFINED,
		                            width,
		                            height,
		                            SDL_WINDOW_OPENGL);

    spdlog::info("SDL: created window of size {}x{} and title \"{}\"", width, height, title);

    return window;
}

void platform::loop(std::function<bool(SDL_Event&)> loop_function) {
    SDL_Event event;
    while(true) {
        if(loop_function(event)) {
            break;
        }
    }
}

platform& platform::get() {
    static platform instance;
    return instance;
}