#pragma once

#include "graphics/vulkan_physical_device.h"

namespace Game {
    struct QueueInfo {
        u32 queueIndex = -1;
        u32 queueFamilyIndex = -1;
    };

    struct DeviceConfig {
        std::string name = "Device";
        PhysicalDevice* physical_device = nullptr;
    };

    struct Device {
        DeviceConfig config{};
        VkDevice device = nullptr;
        VkQueue graphicsQueue = nullptr;
        VkQueue presentQueue = nullptr;

        operator VkDevice() const {
            return device;
        }

        VkDevice vk() const {
            return device;
        }

        VkDevice* vk_ptr() {
            return &device;
        }
    };

    Device create_vulkan_device(const DeviceConfig& config);

    void destroy_vulkan_device(const Device& device);

    void set_vulkan_object_name(const Device& device, void* object, VkObjectType object_type, const char* object_name);
}
