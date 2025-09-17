#pragma once

#include "graphics/device.h"
#include "graphics/physical_device.h"
#include "window/window.h"

#include <vulkan/vulkan.h>

#define GM_VK_ALLOCATOR nullptr
#define GM_ASSERT_VK_SUCCESS(expression, message) GM_ASSERT(expression == VK_SUCCESS, message)
#define GM_ASSERT_THROW_VK_SUCCESS(expression, message) GM_ASSERT_THROW(expression == VK_SUCCESS, message)

namespace Game {

    struct VulkanConfig {
        std::string application_name;
        std::string engine_name;
        bool validation_layers_enabled = false;
        Window* window = nullptr;
    };

    struct Vulkan {
        VulkanConfig config{};
        VkInstance instance = nullptr;
        std::vector<const char*> extensions;
        std::vector<const char*> validation_layers;
        VkDebugUtilsMessengerEXT debug_messenger = nullptr;
        VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{};

        VkSurfaceKHR surface = nullptr;
        physical_device physical_device;
        VulkanDevice device;
    };

    Vulkan create_vulkan(const VulkanConfig& config);

    void destroy_vulkan(Vulkan& vulkan);

    VkResult create_vulkan_instance(Vulkan& vulkan);

    void destroy_vulkan_instance(const Vulkan& vulkan);

    VkResult create_vulkan_debug_messenger(Vulkan& vulkan);

    void destroy_vulkan_debug_messenger(const Vulkan& vulkan);

    VkResult create_vulkan_surface(Vulkan& vulkan);

    void destroy_vulkan_surface(const Vulkan& vulkan);

    bool is_vulkan_supported();

    std::vector<const char*> get_required_vulkan_extensions(const Vulkan& vulkan);

    std::vector<VkExtensionProperties> get_available_vulkan_extensions();

    bool has_vulkan_extensions(const std::vector<const char*>& extensions);

    std::vector<const char*> get_required_vulkan_validation_layers();

    std::vector<VkLayerProperties> get_available_vulkan_validation_layers();

    bool has_vulkan_validation_layers(const std::vector<const char*>& validation_layers);

    VkDebugUtilsMessengerCreateInfoEXT get_vulkan_debug_messenger_create_info();

    VKAPI_ATTR VkBool32 VKAPI_CALL on_vulkan_debug_message(
            VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
            VkDebugUtilsMessageTypeFlagsEXT message_type,
            const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
            void* user_data
    );
}
