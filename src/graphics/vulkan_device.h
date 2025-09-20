#pragma once

#include "vulkan.h"

namespace Game {
    struct QueueInfo {
        u32 queueIndex = -1;
        u32 queueFamilyIndex = -1;
    };

    struct DeviceConfig {
        std::string name = "LogicalDevice";
    };

    void create_vulkan_device(Vulkan& vulkan, const DeviceConfig& config = {});

    void destroy_vulkan_device(const Vulkan& vulkan);

    void set_vulkan_object_name(VkDevice device, void* object, VkObjectType object_type, const char* object_name);
}
