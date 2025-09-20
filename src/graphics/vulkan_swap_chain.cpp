#include "vulkan_swap_chain.h"
#include "vulkan_device.h"

namespace Game {
    std::vector<VkImage> get_images(VkDevice device, VkSwapchainKHR swap_chain) {
        u32 image_count = 0;
        if (vkGetSwapchainImagesKHR(device, swap_chain, &image_count, nullptr) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan swap chain image count");
        }
        std::vector<VkImage> images(image_count);
        if (vkGetSwapchainImagesKHR(device, swap_chain, &image_count, images.data()) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan swap chain images");
        }
        return images;
    }

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

    VkExtent2D get_image_extent(const VkSurfaceCapabilitiesKHR& surface_capabilities, const WindowSize& window_size) {
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

    void create_vulkan_swap_chain(Vulkan& vulkan, const SwapChainConfig& config) {
        VkSurfaceFormatKHR surface_format = get_surface_format(vulkan.physical_device_surface_formats);
        VkPresentModeKHR present_mode = get_present_mode(vulkan.physical_device_present_modes);
        VkExtent2D image_extent = get_image_extent(vulkan.physical_device_surface_capabilities, get_window_size(*config.window));
        u32 min_image_count = get_min_image_count(vulkan.physical_device_surface_capabilities);

        vulkan.swap_chain_surface_format = surface_format;
        vulkan.swap_chain_extent = image_extent;

        VkSwapchainCreateInfoKHR swap_chain_create_info{};
        swap_chain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swap_chain_create_info.minImageCount = min_image_count;
        swap_chain_create_info.surface = vulkan.surface;
        swap_chain_create_info.imageFormat = surface_format.format;
        swap_chain_create_info.imageColorSpace = surface_format.colorSpace;
        swap_chain_create_info.imageExtent = image_extent;
        swap_chain_create_info.imageArrayLayers = 1;
        swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swap_chain_create_info.preTransform = vulkan.physical_device_surface_capabilities.currentTransform;
        swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swap_chain_create_info.presentMode = present_mode;
        swap_chain_create_info.clipped = VK_TRUE;
        swap_chain_create_info.oldSwapchain = nullptr;

        const QueueFamilyIndices& queue_family_indices = vulkan.physical_device_queue_family_indices;
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

        if (vkCreateSwapchainKHR(vulkan.device, &swap_chain_create_info, GM_VK_ALLOCATOR, &vulkan.swap_chain) != VK_SUCCESS) {
            GM_THROW("Could not create Vulkan swap chain");
        }

        set_vulkan_object_name(vulkan.device, vulkan.swap_chain, VK_OBJECT_TYPE_SWAPCHAIN_KHR, config.name.c_str());

        vulkan.swap_chain_images = get_images(vulkan.device, vulkan.swap_chain);
        u32 image_count = vulkan.swap_chain_images.size();
        if (image_count < min_image_count) {
            GM_THROW("Could not get minimum required swap chain images. Got [" << image_count << "], required [" << min_image_count << "]");
        }

        vulkan.swap_chain_image_views.resize(image_count);
        for (u32 i = 0; i < image_count; ++i) {
            VkImageViewCreateInfo image_view_create_info{};
            image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            image_view_create_info.image = vulkan.swap_chain_images[i];
            image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            image_view_create_info.format = surface_format.format;
            image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            image_view_create_info.subresourceRange.baseMipLevel = 0;
            image_view_create_info.subresourceRange.levelCount = 1;
            image_view_create_info.subresourceRange.baseArrayLayer = 0;
            image_view_create_info.subresourceRange.layerCount = 1;

            if (vkCreateImageView(vulkan.device, &image_view_create_info, GM_VK_ALLOCATOR, &vulkan.swap_chain_image_views[i]) != VK_SUCCESS) {
                GM_THROW("Could not create Vulkan swap chain image view [" << i + 1 << " / " << image_count << "]");
            }
        }
    }

    void destroy_vulkan_swap_chain(const Vulkan& vulkan) {
        for (VkImageView image_view : vulkan.swap_chain_image_views) {
            vkDestroyImageView(vulkan.device, image_view, GM_VK_ALLOCATOR);
        }
        if (vulkan.swap_chain) {
            vkDestroySwapchainKHR(vulkan.device, vulkan.swap_chain, GM_VK_ALLOCATOR);
        }
    }
}
