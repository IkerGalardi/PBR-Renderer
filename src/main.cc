#include <iostream>

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "platform.hh"

int main(int argc, char** argv) {
    platform::get().create_window(1000, 1000, "test");

    // Set the application information
    vk::ApplicationInfo app_info;
    app_info.sType = vk::StructureType::eApplicationInfo;
    app_info.pApplicationName = "PBR-Renderer";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "PBR-Renderer";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    // Setup basic instance create info
    vk::InstanceCreateInfo instance_info;
    instance_info.sType = vk::StructureType::eInstanceCreateInfo;
    instance_info.pApplicationInfo = &app_info;

    // Setup extensions in instance create info
    uint32_t glfw_extension_count = 0;
    const char** glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    instance_info.enabledExtensionCount = glfw_extension_count;
    instance_info.ppEnabledExtensionNames = glfw_extensions;
    instance_info.enabledLayerCount = 0;

    // Create instance
    vk::Instance instance;
    vk::Result result = vk::createInstance(&instance_info, nullptr, &instance);
    if(result != vk::Result::eSuccess) {
        spdlog::error("Vulkan: error creating instance");
        std::exit(2);
    }
    
    platform::get().loop([&]() {
        
    });
}