#pragma once

#include "graphics/vulkan_device.h"
#include "graphics/vulkan_instance.h"
#include "graphics/vulkan_physical_device.h"
#include "window/window.h"

namespace Game {

    struct VulkanConfig {
        std::string application_name;
        std::string engine_name;
        bool validation_layers_enabled = false;
        Window* window = nullptr;
    };

    struct Vulkan {
        VulkanConfig config{};
        VulkanInstance instance{};
        PhysicalDevice physical_device{};
        Device device{};
    };

    Vulkan create_vulkan(const VulkanConfig& config);

    void destroy_vulkan(const Vulkan& vulkan);
}
