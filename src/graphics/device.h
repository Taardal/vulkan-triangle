#pragma once

#include <vulkan/vulkan.h>

namespace Game {

    // Forward declaration
    struct Vulkan;

    struct QueueInfo {
        u32 queueIndex = -1;
        u32 queueFamilyIndex = -1;
    };

    struct VulkanDevice {
        VkDevice device = nullptr;
        VkQueue graphicsQueue = nullptr;
        VkQueue presentQueue = nullptr;

        operator VkDevice() const;

        VkDevice* vk_ptr();
    };

    void create_device(Vulkan& vulkan);

    void destroy_device(Vulkan& vulkan);

    VkQueue get_device_queue(Vulkan& vulkan, uint32_t queue_family_index);
}
