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

    spdlog::info("Vulkan: instance and debug messager created");

    // Create surface
    VkSurfaceKHR surface;
    SDL_Vulkan_CreateSurface(window, instance, &surface);

    spdlog::info("SDL: created vulkan surface");

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

    spdlog::info("Vulkan: created logical and physical devices");

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

    spdlog::info("Vulkan: created swapchain and views");

    VkQueue graphics_queue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
    unsigned int graphics_queue_index = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

    // Create graphics command pool
    VkCommandPoolCreateInfo pool_create_info = {};
    pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info.pNext = nullptr;
    pool_create_info.queueFamilyIndex = graphics_queue_index;
    pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VkCommandPool graphics_command_pool;
    vkCreateCommandPool(device, &pool_create_info, nullptr, &graphics_command_pool);
    spdlog::info("Vulkan: created graphics command pool");

    VkCommandBufferAllocateInfo command_buffer_info = {};
    command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_info.pNext = nullptr;
    command_buffer_info.commandPool = graphics_command_pool;
    command_buffer_info.commandBufferCount = 1;
    command_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(device, &command_buffer_info, &command_buffer);

    platform::get().loop([&](SDL_Event& event) {
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                spdlog::info("SDL: quit event arrived");
                return true;
            }
        }

        return false;
    });

    // Cleanup
    vkDestroySwapchainKHR(device, swapchain, nullptr);
    for(auto& view : swapchain_views) {
        vkDestroyImageView(device, view, nullptr);
    }
    spdlog::info("Vulkan: swapchain destroyed");

    vkDestroyCommandPool(device, graphics_command_pool, nullptr);
    spdlog::info("Vulkan: graphics command pool destroyed");
    
    vkDestroyDevice(device, nullptr);
    spdlog::info("Vulkan: device destroyed");

    vkDestroySurfaceKHR(instance, surface, nullptr);
    spdlog::info("Vulkan: surface destroyed");

    vkb::destroy_debug_utils_messenger(instance, debug_messenger);
    spdlog::info("Vulkan: debug messager destroyed");

    vkDestroyInstance(instance, nullptr);
    spdlog::info("Vulkan: destroyed instance");

    SDL_DestroyWindow(window);
    spdlog::info("SDL: window destroyed");
}