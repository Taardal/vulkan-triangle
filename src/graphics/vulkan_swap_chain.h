#pragma once

#include "graphics/vulkan_physical_device.h"
#include "window/window.h"

namespace Game {
    struct SwapChainConfig {
        std::string name = "SwapChain";
        Window* window = nullptr;
        VkDevice device = nullptr;
        VkSurfaceKHR surface = nullptr;
        SwapChainInfo swap_chain_info{};
        QueueFamilyIndices queue_family_indices{};
    };

    struct SwapChain {
        SwapChainConfig config{};
        VkSwapchainKHR swap_chain = nullptr;
        VkSurfaceFormatKHR surface_format{};
        VkPresentModeKHR present_mode;
        VkExtent2D extent{};
        std::vector<VkImage> images;
        u32 image_count = 0;

        operator VkSwapchainKHR() const {
            return swap_chain;
        }

        VkSwapchainKHR vk() const {
            return swap_chain;
        }

        VkSwapchainKHR* vk_ptr() {
            return &swap_chain;
        }
    };

    SwapChain create_vulkan_swap_chain(const SwapChainConfig& config);

    void destroy_vulkan_swap_chain(const SwapChain& swap_chain);
}