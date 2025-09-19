#pragma once

#include "window/window.h"

namespace Game {
    struct VulkanInstanceConfig {
        std::string application_name;
        std::string engine_name;
        bool validation_layers_enabled = false;
        Window* window = nullptr;
    };

    struct VulkanInstance {
        VulkanInstanceConfig config{};
        VkDebugUtilsMessengerEXT debug_messenger = nullptr;
        std::vector<const char*> extensions;
        VkInstance instance = nullptr;
        VkSurfaceKHR surface = nullptr;
        std::vector<const char*> validation_layers;

        operator VkInstance() const {
            return instance;
        }

        VkInstance vk() const {
            return instance;
        }

        VkInstance* vk_ptr() {
            return &instance;
        }
    };

    VulkanInstance create_vulkan_instance(const VulkanInstanceConfig& config);

    void destroy_vulkan_instance(const VulkanInstance& vulkan_instance);
}
