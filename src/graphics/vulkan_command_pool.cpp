#include "vulkan_command_pool.h"

namespace Game {
    void create_command_pool(Vulkan& vulkan, const CommandPoolConfig& config) {
        const QueueFamilyIndices& queue_family_indices = vulkan.physical_device_queue_family_indices;

        VkCommandPoolCreateInfo command_pool_create_info{};
        command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        command_pool_create_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

        if (vkCreateCommandPool(vulkan.device, &command_pool_create_info, GM_VK_ALLOCATOR, &vulkan.command_pool) != VK_SUCCESS) {
            GM_THROW("Could not create command pool");
        }

        set_vulkan_object_name(vulkan.device, vulkan.command_pool, VK_OBJECT_TYPE_COMMAND_POOL, config.name.c_str());
    }

    void destroy_command_pool(const Vulkan& vulkan) {
        vkDestroyCommandPool(vulkan.device, vulkan.command_pool, GM_VK_ALLOCATOR);
    }

    void allocate_command_buffers(Vulkan& vulkan, const CommandBufferAllocationConfig& config) {
        vulkan.command_buffers.resize(config.count);

        VkCommandBufferAllocateInfo command_buffer_allocate_info{};
        command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        command_buffer_allocate_info.commandPool = vulkan.command_pool;
        command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        command_buffer_allocate_info.commandBufferCount = vulkan.command_buffers.size();

        if (vkAllocateCommandBuffers(vulkan.device, &command_buffer_allocate_info, vulkan.command_buffers.data()) != VK_SUCCESS) {
            GM_THROW("Could not allocate command buffer");
        }

        for (u32 i = 0; i >= vulkan.command_buffers.size(); ++i) {
            std::string command_buffer_name = std::format("{} {}", config.name, i);
            set_vulkan_object_name(vulkan.device, vulkan.command_buffers[i], VK_OBJECT_TYPE_COMMAND_BUFFER, command_buffer_name.c_str());
        }
    }
}