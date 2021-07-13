#include <iostream>
#include <vector>

#include <spdlog/spdlog.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vkbootstrap/VkBootstrap.h>

#include "platform.hh"

int main(int argc, char** argv) {
    auto* window = platform::get().create_window(1000, 1000, "test");


    vkb::InstanceBuilder builder;

    //make the Vulkan instance, with basic debug features
    auto inst_ret = builder.set_app_name("Example Vulkan Application")
                        .request_validation_layers(true)
                        .require_api_version(1, 1, 0)
                        .use_default_debug_messenger()
                        .build();

    vkb::Instance vkb_inst = inst_ret.value();

    VkInstance instance = vkb_inst.instance;
    VkDebugUtilsMessengerEXT debug_messenger = vkb_inst.debug_messenger;

    // Create surface
    VkSurfaceKHR surface;
    SDL_Vulkan_CreateSurface(window, instance, &surface);

    //use vkbootstrap to select a GPU.
    //We want a GPU that can write to the SDL surface and supports Vulkan 1.1
    vkb::PhysicalDeviceSelector selector{vkb_inst};
    vkb::PhysicalDevice physicalDevice = selector
                                             .set_minimum_version(1, 1)
                                             .set_surface(surface)
                                             .select()
                                             .value();

    //create the final Vulkan device
    vkb::DeviceBuilder deviceBuilder{physicalDevice};

    vkb::Device vkbDevice = deviceBuilder.build().value();

    // Get the VkDevice handle used in the rest of a Vulkan application
    VkDevice device = vkbDevice.device;
    VkPhysicalDevice physical_device = physicalDevice.physical_device;

    VkSwapchainKHR swapchain;
    VkFormat image_format;
    std::vector<VkImage> swapchain_images;
    std::vector<VkImageView> swapchain_views;

	vkb::SwapchainBuilder swapchainBuilder{physical_device, device, surface };

	vkb::Swapchain vkbSwapchain = swapchainBuilder
		.use_default_format_selection()
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(1000, 1000)
		.build()
		.value();

	//store swapchain and its related images
	swapchain = vkbSwapchain.swapchain;
	swapchain_images = vkbSwapchain.get_images().value();
	swapchain_views = vkbSwapchain.get_image_views().value();
	image_format = vkbSwapchain.image_format;

    platform::get().loop([&](SDL_Event event) {
        
    });

    // Cleanup
    vkDestroySwapchainKHR(device, swapchain, nullptr);
    for(auto& view : swapchain_views) {
        vkDestroyImageView(device, view, nullptr);
    }
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkb::destroy_debug_utils_messenger(instance, debug_messenger);
    vkDestroyInstance(instance, nullptr);
    SDL_DestroyWindow(window);
}