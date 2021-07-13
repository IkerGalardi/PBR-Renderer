#include "platform.hh"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

platform::platform() {
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%l] %v%$");

    spdlog::info("Logging prepared.");

    if(!glfwInit()) {
        spdlog::error("GLFW: could not be initialized");
        std::exit(1);
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    spdlog::info("GLFW: initialized");
}

platform::~platform() {
    glfwDestroyWindow(window);
}


void platform::create_window(unsigned int width, unsigned int height, const std::string& title) {
    this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    spdlog::info("GLFW: created window of size {}x{} and title \"{}\"", width, height, title);
}

void platform::loop(std::function<void(void)> loop_function) {
    while(!glfwWindowShouldClose(this->window)) {
        loop_function();

        glfwPollEvents();
    }
}

platform& platform::get() {
    static platform instance;
    return instance;
}