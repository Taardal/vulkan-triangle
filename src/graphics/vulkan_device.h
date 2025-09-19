#pragma once

#include "graphics/vulkan_physical_device.h"

namespace Game {
    struct QueueInfo {
        u32 queueIndex = -1;
        u32 queueFamilyIndex = -1;
    };

    struct DeviceConfig {
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

    Device create_device(const DeviceConfig& config);

    void destroy_device(const Device& device);
}
