#include "renderer.hh"

#include <vector>

#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vkbootstrap/VkBootstrap.h>
#include <spdlog/spdlog.h>

// Basic vulkan handlers
VkInstance instance;
VkDebugUtilsMessengerEXT debug_messenger;
VkSurfaceKHR surface;
VkDevice device;
VkPhysicalDevice physical_device;

// Framebuffer and swapchain related handlers
VkSwapchainKHR swapchain;
VkFormat image_format;
std::vector<VkImage> swapchain_images;
std::vector<VkImageView> swapchain_views;
std::vector<VkFramebuffer> swapchain_framebuffers;

// Graphics command queue and pool
unsigned int graphics_command_queue_index;
VkQueue graphics_command_queue;
VkCommandPool graphics_command_pool;
VkCommandBuffer graphics_command_buffer;
VkRenderPass render_pass;

// Sinchronization stuff
VkFence fence;
VkSemaphore present_semaphore;
VkSemaphore render_semaphore;

static void initialize_vulkan(SDL_Window* window) {
    vkb::InstanceBuilder builder;
    auto inst_ret = builder.set_app_name("Example Vulkan Application")
                        .request_validation_layers(true)
                        .require_api_version(1, 1, 0)
                        .use_default_debug_messenger()
                        .build();

    if(!inst_ret.has_value()) {
        
        spdlog::error("Renderer: error while creating the instance");
        std::exit(2);
    }

    vkb::Instance vkb_inst = inst_ret.value();

    instance = vkb_inst.instance;
    debug_messenger = vkb_inst.debug_messenger;

    spdlog::info("Renderer: instance and debug messager created");

    SDL_Vulkan_CreateSurface(window, instance, &surface);

    spdlog::info("SDL: created vulkan surface");

    vkb::PhysicalDeviceSelector selector{vkb_inst};
    vkb::PhysicalDevice physicalDevice = selector
                                             .set_minimum_version(1, 1)
                                             .set_surface(surface)
                                             .select()
                                             .value();

    vkb::DeviceBuilder deviceBuilder{physicalDevice};
    vkb::Device vkbDevice = deviceBuilder.build().value();

    device = vkbDevice.device;
    physical_device = physicalDevice.physical_device;

    spdlog::info("Renderer: created logical and physical devices");

    vkb::SwapchainBuilder swapchainBuilder{physical_device, device, surface};
    vkb::Swapchain vkbSwapchain = swapchainBuilder
                                    .use_default_format_selection()
                                    .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
                                    .set_desired_extent(1000, 1000)
                                    .build()
                                    .value();
    swapchain = vkbSwapchain.swapchain;
    swapchain_images = vkbSwapchain.get_images().value();
    swapchain_views = vkbSwapchain.get_image_views().value();
    image_format = vkbSwapchain.image_format;

    spdlog::info("Renderer: created swapchain and views");

    
    graphics_command_queue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
    graphics_command_queue_index = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
}

static void create_render_and_sub_passes() {
    // Create a color attatchment description
    VkAttachmentDescription color_attachment = {};
    color_attachment.format = image_format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Create a color attatchment reference
    VkAttachmentReference color_attachment_ref = {};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Create a sub render pass (the minimum required)
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkRenderPassCreateInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass);
}


namespace renderer {
    void initialize(SDL_Window* window) {
        // Using VkBootstrap create the vulkan instance and select a device
        initialize_vulkan(window);

        // Create graphics command pool
        VkCommandPoolCreateInfo pool_create_info = {};
        pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_create_info.pNext = nullptr;
        pool_create_info.queueFamilyIndex = graphics_command_queue_index;
        pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        vkCreateCommandPool(device, &pool_create_info, nullptr, &graphics_command_pool);

        spdlog::info("Renderer: graphics command queue created");

        // Create a command buffer
        VkCommandBufferAllocateInfo command_buffer_info = {};
        command_buffer_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_info.pNext = nullptr;
        command_buffer_info.commandPool = graphics_command_pool;
        command_buffer_info.commandBufferCount = 1;
        command_buffer_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        vkAllocateCommandBuffers(device, &command_buffer_info, &graphics_command_buffer);

        create_render_and_sub_passes();

        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        // Create a template framebuffer create info
        VkFramebufferCreateInfo framebuffer_create_info = {};
        framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info.pNext = nullptr;
        framebuffer_create_info.renderPass = render_pass;
        framebuffer_create_info.width = width;
        framebuffer_create_info.height = height;
        framebuffer_create_info.attachmentCount = 1;
        framebuffer_create_info.layers = 1;
        swapchain_framebuffers.resize(swapchain_views.size());
        for(int i = 0; i < swapchain_views.size(); i++) {
            framebuffer_create_info.pAttachments = &swapchain_views[i];
            vkCreateFramebuffer(device, &framebuffer_create_info, nullptr, &swapchain_framebuffers[i]);
        }

        spdlog::info("Created all the framebuffers");

        // Create a fence
        VkFenceCreateInfo fence_info = {};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.pNext = nullptr;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(device, &fence_info, nullptr, &fence);

        // Create render and present semaphores
	    VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	    semaphoreCreateInfo.pNext = nullptr;
	    semaphoreCreateInfo.flags = 0;
	    vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &present_semaphore);
	    vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &render_semaphore);
    }

    void render() {
        // Wait for render to finish
        vkWaitForFences(device, 1, &fence, true, 10000);
        vkResetFences(device, 1, &fence);
        spdlog::trace("Renderer: waited to fence");

        // Reset the command buffer to start using it
        vkResetCommandBuffer(graphics_command_buffer, 0);
        spdlog::trace("Renderer: command buffer reset");

        unsigned int next_image_index;
        vkAcquireNextImageKHR(device, swapchain, 10000, present_semaphore, fence, &next_image_index);
        spdlog::trace("Renderer: swapchain image acquired");

        vkWaitForFences(device, 1, &fence, true, 10000);
        vkResetFences(device, 1, &fence);

        VkCommandBufferBeginInfo command_buffer_begin_info = {};
        command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        command_buffer_begin_info.pNext = nullptr;
        command_buffer_begin_info.pInheritanceInfo = nullptr;
        command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(graphics_command_buffer, &command_buffer_begin_info);

        spdlog::trace("Renderer: command buffer started recording");


        // Set the clear color
        VkClearValue clear_value;
        clear_value.color = {{0.0f, 1.0f, 0.0f, 1.0f}};

        VkRenderPassBeginInfo pass_begin_info = {};
        pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        pass_begin_info.pNext = nullptr;
        pass_begin_info.framebuffer = swapchain_framebuffers[next_image_index];
        pass_begin_info.renderPass = render_pass;
        pass_begin_info.renderArea.offset = {0, 0};
        pass_begin_info.renderArea.extent = {1000, 1000};
        pass_begin_info.clearValueCount = 1;
        pass_begin_info.pClearValues = &clear_value;
        vkCmdBeginRenderPass(graphics_command_buffer, &pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
        spdlog::trace("Renderer: render pass started");
        vkCmdEndRenderPass(graphics_command_buffer);
        spdlog::trace("Renderer: render pass ended");
        vkEndCommandBuffer(graphics_command_buffer);
        spdlog::trace("Renderer: command buffer recording ended");

        VkSubmitInfo submit = {};
        submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit.pNext = nullptr;

        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submit.pWaitDstStageMask = &wait_stage;
        submit.waitSemaphoreCount = 1;
        submit.pWaitSemaphores = &present_semaphore;
        submit.signalSemaphoreCount = 1;
        submit.pSignalSemaphores = &render_semaphore;
        submit.commandBufferCount = 1;
        submit.pCommandBuffers = &graphics_command_buffer;
        vkQueueSubmit(graphics_command_queue, 1, &submit, fence);
        spdlog::trace("Renderer: command stream submited");

        vkWaitForFences(device, 1, &fence, true, 10000);
        vkResetFences(device, 1, &fence);

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = nullptr;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.swapchainCount = 1;
        presentInfo.pWaitSemaphores = &render_semaphore;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pImageIndices = &next_image_index;
        vkQueuePresentKHR(graphics_command_queue, &presentInfo);
        spdlog::trace("Renderer: presented image");
    }

    void destroy() {
        vkDestroySwapchainKHR(device, swapchain, nullptr);
        for(int i = 0; i < swapchain_images.size(); i++) {
            vkDestroyImage(device, swapchain_images[i], nullptr);
            vkDestroyImageView(device, swapchain_views[i], nullptr);
			vkDestroyFramebuffer(device, swapchain_framebuffers[i], nullptr);
        }
		for (int i = 0; i < swapchain_framebuffers.size(); i++) {
		}
        spdlog::info("Renderer: swapchain destroyed");

        vkDestroyCommandPool(device, graphics_command_pool, nullptr);
        spdlog::info("Renderer: graphics command pool destroyed");
    
        vkDestroyDevice(device, nullptr);
        spdlog::info("Renderer: device destroyed");

        vkDestroySurfaceKHR(instance, surface, nullptr);
        spdlog::info("Renderer: surface destroyed");
        vkb::destroy_debug_utils_messenger(instance, debug_messenger);
        spdlog::info("Renderer: debug messager destroyed");

        vkDestroyInstance(instance, nullptr);
        spdlog::info("Renderer: destroyed instance");
    }
}