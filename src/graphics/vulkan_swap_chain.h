#pragma once

#include "vulkan.h"
#include "window/window.h"

namespace Game {
    struct SwapChainConfig {
        std::string name = "SwapChain";
        Window* window = nullptr;
    };

    void create_vulkan_swap_chain(Vulkan& vulkan, const SwapChainConfig& config);

    void destroy_vulkan_swap_chain(const Vulkan& vulkan);
}
