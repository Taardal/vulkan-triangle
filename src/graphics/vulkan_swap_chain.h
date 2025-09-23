#pragma once

#include "vulkan.h"
#include "window/window.h"

namespace Game {
    struct SwapChainConfig {
        Window* window = nullptr;
        std::string name = "SwapChain";
        u32 image_count = 0;
    };

    void create_vulkan_swap_chain(Vulkan& vulkan, const SwapChainConfig& config);

    void destroy_vulkan_swap_chain(const Vulkan& vulkan);

    void recreate_vulkan_swap_chain(Vulkan& vulkan, const SwapChainConfig& config);
}
