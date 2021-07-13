#pragma once

#include <string>
#include <functional>

#include <SDL2/SDL.h>

class platform {
public:
    SDL_Window* create_window(unsigned int width, unsigned int height, const std::string& title);

    void loop(std::function<bool(SDL_Event&)> loop_function);

    static platform& get();
private:
    platform();
    ~platform();

    SDL_Window* window;
protected:
};