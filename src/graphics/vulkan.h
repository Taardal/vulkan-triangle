#pragma once

#include "window/window.h"

namespace Game {

    struct QueueFamilyIndices {
        std::optional<u32> graphics_family;
        std::optional<u32> present_family;
    };

    struct VulkanConfig {
        Window* window = nullptr;
        std::string application_name;
        std::string engine_name;
        bool validation_layers_enabled = false;
        bool debug_extension_enabled = false;
    };

    struct Vulkan {
        VkInstance instance = nullptr;
        VkDebugUtilsMessengerEXT debug_messenger = nullptr;

        VkSurfaceKHR surface = nullptr;

        VkPhysicalDevice physical_device = nullptr;
        VkPhysicalDeviceProperties physical_device_properties{};
        VkPhysicalDeviceFeatures physical_device_features{};
        std::vector<VkExtensionProperties> physical_device_extensions{};
        VkSurfaceCapabilitiesKHR physical_device_surface_capabilities{};
        std::vector<VkSurfaceFormatKHR> physical_device_surface_formats;
        std::vector<VkPresentModeKHR> physical_device_present_modes;
        QueueFamilyIndices physical_device_queue_family_indices{};
        VkFormat physical_device_depth_format = VK_FORMAT_UNDEFINED;

        VkDevice device = nullptr;
        VkQueue graphics_queue = nullptr;
        VkQueue present_queue = nullptr;

        VkSwapchainKHR swap_chain = nullptr;
        VkExtent2D swap_chain_extent{};
        VkSurfaceFormatKHR swap_chain_surface_format{};
        std::vector<VkImage> swap_chain_images;
        std::vector<VkImageView> swap_chain_image_views;
        std::vector<VkFramebuffer> swap_chain_framebuffers;
        VkRenderPass swap_chain_render_pass = nullptr;

        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipeline_layout = nullptr;
        VkShaderModule vertex_shader = nullptr;
        VkShaderModule fragment_shader = nullptr;

        VkCommandPool command_pool = nullptr;
        VkCommandBuffer command_buffer = nullptr;
        std::vector<VkCommandBuffer> command_buffers;

        VkSemaphore image_available_semaphore;
        VkSemaphore render_finished_semaphore;
        VkFence in_flight_fence;
    };

    void create_vulkan(Vulkan& vulkan, const VulkanConfig& config);

    void destroy_vulkan(const Vulkan& vulkan);

    void render_frame(Vulkan& vulkan);
}
