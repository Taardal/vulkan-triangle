#pragma once

#include <vulkan/vulkan.h>

namespace dd {

    // Forward declaration
    struct vulkan;

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

    void create_device(vulkan& vulkan);

    void destroy_device(vulkan& vulkan);

    VkQueue get_device_queue(vulkan& vulkan, uint32_t queue_family_index);
}
