#include <iostream>

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

int main(int argc, char** argv) {
    // Prepare SPDLOG
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


    
    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Test", nullptr, nullptr);
    if(!window) {
        spdlog::error("GLFW: window could not be created");
        std::exit(1);
    }
    spdlog::info("GLFW: 1000x1000 window created");
    
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}