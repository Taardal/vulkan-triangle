#pragma once

#include "graphics/device.h"
#include "graphics/physical_device.h"
#include "window/window.h"

#include <vulkan/vulkan.h>

#define DD_VK_ALLOCATOR nullptr
#define DD_ASSERT_VK_SUCCESS(expression, message) DD_ASSERT(expression == VK_SUCCESS, message)
#define DD_ASSERT_THROW_VK_SUCCESS(expression, message) DD_ASSERT_THROW(expression == VK_SUCCESS, message)

namespace dd {

    struct vulkan_config {
        window* window = nullptr;
        std::string application_name;
        std::string engine_name;
        bool validation_layers_enabled = false;
    };

    struct vulkan {
        vulkan_config config;
        VkInstance instance = nullptr;
        VkDebugUtilsMessengerEXT debug_messenger = nullptr;
        VkSurfaceKHR surface = nullptr;
        physical_device physical_device;
        VulkanDevice device;

        explicit vulkan(vulkan_config config) : config(std::move(config)) {}
    };

    void initialize_vulkan(vulkan& vulkan);

    void terminate_vulkan(vulkan& vulkan);

    void create_vulkan_instance(
        vulkan& vulkan,
        const std::vector<const char*>& required_extensions,
        const std::vector<const char*>& validation_layers,
        const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info
    );

    void destroy_vulkan_instance(vulkan& vulkan);

    void create_debug_messenger(vulkan& vulkan, const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info);

    void destroy_debug_messenger(vulkan& vulkan);

    void create_surface(vulkan& vulkan);

    void destroy_surface(vulkan& vulkan);

    bool is_vulkan_supported();

    std::vector<const char*> get_required_extensions();

    std::vector<VkExtensionProperties> get_available_extensions();

    bool has_extensions(const std::vector<const char*>& extensions);

    std::vector<VkLayerProperties> get_available_validation_layers();

    bool has_validation_layers(const std::vector<const char*>& validation_layers);

    VkDebugUtilsMessengerCreateInfoEXT get_debug_messenger_create_info();

    VKAPI_ATTR VkBool32 VKAPI_CALL on_vulkan_debug_message(
            VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
            VkDebugUtilsMessageTypeFlagsEXT message_type,
            const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
            void* user_data
    );
}
