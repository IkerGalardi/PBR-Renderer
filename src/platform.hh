#pragma once

#include <string>
#include <functional>

#include <GLFW/glfw3.h>

class platform {
public:
    void create_window(unsigned int width, unsigned int height, const std::string& title);

    void loop(std::function<void(void)> loop_function);

    static platform& get();
private:
    platform();
    ~platform();

    GLFWwindow* window;
protected:
};