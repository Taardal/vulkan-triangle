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
    };

    struct Vulkan {
        VkDebugUtilsMessengerEXT debug_messenger = nullptr;
        VkDevice device = nullptr;
        VkInstance instance = nullptr;
        VkQueue graphicsQueue = nullptr;
        VkQueue presentQueue = nullptr;
        VkPhysicalDevice physical_device = nullptr;
        VkPhysicalDeviceProperties physical_device_properties{};
        VkPhysicalDeviceFeatures physical_device_features{};
        std::vector<VkExtensionProperties> physical_device_extensions{};
        VkSurfaceCapabilitiesKHR physical_device_surface_capabilities{};
        std::vector<VkSurfaceFormatKHR> physical_device_surface_formats;
        std::vector<VkPresentModeKHR> physical_device_present_modes;
        QueueFamilyIndices physical_device_queue_family_indices{};
        VkFormat physical_device_depth_format = VK_FORMAT_UNDEFINED;
        VkSurfaceKHR surface = nullptr;
        VkSwapchainKHR swap_chain = nullptr;
        VkExtent2D swap_chain_extent{};
        std::vector<VkImage> swap_chain_images;
        std::vector<VkImageView> swap_chain_image_views;
        VkSurfaceFormatKHR swap_chain_surface_format{};
    };

    void create_vulkan(Vulkan& vulkan, const VulkanConfig& config);

    void destroy_vulkan(const Vulkan& vulkan);
}
