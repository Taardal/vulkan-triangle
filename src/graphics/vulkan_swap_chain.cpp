#include "vulkan_swap_chain.h"
#include "vulkan_device.h"

namespace Game {
    u32 get_min_image_count(const VkSurfaceCapabilitiesKHR& surface_capabilities) {
        u32 min_image_count = surface_capabilities.minImageCount;
        u32 max_image_count = surface_capabilities.maxImageCount;

        // Simply sticking to the surface capability minimum means that we may sometimes have to wait on the driver to
        // complete internal operations before we can acquire another image to render to. Therefore, it is recommended
        // to request at least one more image than the minimum.
        u32 image_count = min_image_count + 1;

        if (max_image_count > 0 && image_count > max_image_count) {
            image_count = max_image_count;
        }
        return image_count;
    }

    VkSurfaceFormatKHR get_surface_format(const std::vector<VkSurfaceFormatKHR>& surface_formats) {
        for (const auto& surface_format : surface_formats) {
            if (surface_format.format == VK_FORMAT_B8G8R8A8_SRGB && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return surface_format;
            }
        }
        return surface_formats[0];
    }

    VkPresentModeKHR get_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D get_extent(const VkSurfaceCapabilitiesKHR& surface_capabilities, const WindowSize& window_size) {
        bool extent_must_match_window_size = surface_capabilities.currentExtent.width != std::numeric_limits<u32>::max();
        if (extent_must_match_window_size) {
            return surface_capabilities.currentExtent;
        }
        const VkExtent2D& min_image_extent = surface_capabilities.minImageExtent;
        const VkExtent2D& max_image_extent = surface_capabilities.maxImageExtent;
        u32 width = std::clamp((u32) window_size.width, min_image_extent.width, max_image_extent.width);
        u32 height = std::clamp((u32) window_size.height, min_image_extent.height, max_image_extent.height);
        return { width, height };
    }

    SwapChain create_vulkan_swap_chain(const SwapChainConfig& config) {
        GM_ASSERT(config.device, "Vulkan device cannot be null");
        GM_ASSERT(config.window, "Window cannot be null");

        SwapChain swap_chain{};
        swap_chain.config = config;

        const SwapChainInfo& swap_chain_info = config.swap_chain_info;
        swap_chain.surface_format = get_surface_format(swap_chain_info.surface_formats);
        swap_chain.present_mode = get_present_mode(swap_chain_info.present_modes);
        swap_chain.extent = get_extent(swap_chain_info.surface_capabilities, get_window_size(*config.window));
        swap_chain.image_count = get_min_image_count(swap_chain_info.surface_capabilities);

        VkSwapchainCreateInfoKHR swap_chain_create_info{};
        swap_chain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swap_chain_create_info.minImageCount = swap_chain.image_count;
        swap_chain_create_info.surface = config.surface;
        swap_chain_create_info.imageFormat = swap_chain.surface_format.format;
        swap_chain_create_info.imageColorSpace = swap_chain.surface_format.colorSpace;
        swap_chain_create_info.imageExtent = swap_chain.extent;
        swap_chain_create_info.imageArrayLayers = 1;
        swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swap_chain_create_info.preTransform = swap_chain_info.surface_capabilities.currentTransform;
        swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swap_chain_create_info.presentMode = swap_chain.present_mode;
        swap_chain_create_info.clipped = VK_TRUE;
        swap_chain_create_info.oldSwapchain = nullptr;

        const QueueFamilyIndices& queue_family_indices = config.queue_family_indices;
        if (queue_family_indices.graphics_family != queue_family_indices.present_family) {
            std::vector<u32> queueFamilyIndexValues = {
                queue_family_indices.graphics_family.value(),
                queue_family_indices.present_family.value()
            };
            swap_chain_create_info.pQueueFamilyIndices = queueFamilyIndexValues.data();
            swap_chain_create_info.queueFamilyIndexCount = queueFamilyIndexValues.size();
            swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        } else {
            swap_chain_create_info.pQueueFamilyIndices = nullptr;
            swap_chain_create_info.queueFamilyIndexCount = 0;
            swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        if (vkCreateSwapchainKHR(config.device, &swap_chain_create_info, GM_VK_ALLOCATOR, swap_chain.vk_ptr()) != VK_SUCCESS) {
            GM_THROW("Could not create Vulkan swap chain");
        }

        set_vulkan_object_name(config.device, swap_chain, VK_OBJECT_TYPE_SWAPCHAIN_KHR, config.name.c_str());

        if (vkGetSwapchainImagesKHR(config.device, swap_chain, &swap_chain.image_count, nullptr) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan swap chain image count");
        }
        swap_chain.images.resize(swap_chain.image_count);
        if (vkGetSwapchainImagesKHR(config.device, swap_chain, &swap_chain.image_count, swap_chain.images.data()) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan swap chain images");
        }

        return swap_chain;
    }

    void destroy_vulkan_swap_chain(const SwapChain& swap_chain) {
        vkDestroySwapchainKHR(swap_chain.config.device, swap_chain, GM_VK_ALLOCATOR);
    }
}
