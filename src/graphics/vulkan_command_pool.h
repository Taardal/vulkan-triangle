#pragma once

#include "vulkan.h"

namespace Game {
    struct CommandPoolConfig {
        std::string name = "CommandPool";
    };

    void create_command_pool(Vulkan& vulkan, const CommandPoolConfig& config);

    void destroy_command_pool(const Vulkan& vulkan);

    struct CommandBufferAllocationConfig {
        std::string name = "CommandBuffer";
        u32 count = 0;
    };

    void allocate_command_buffers(Vulkan& vulkan, const CommandBufferAllocationConfig& config);
}
