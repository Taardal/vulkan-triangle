#pragma once

#include "graphics/vulkan.h"
#include "window/window.h"

namespace Game {
    struct VulkanInstanceConfig {
        std::string application_name;
        std::string engine_name;
        bool validation_layers_enabled = false;
    };

    void create_vulkan_instance(Vulkan& vulkan, const VulkanInstanceConfig& config);

    void destroy_vulkan_instance(const Vulkan& vulkan);
}
