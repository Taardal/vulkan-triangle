#pragma once

namespace Game {
    struct QueueFamilyIndices {
        std::optional<u32> graphics_family;
        std::optional<u32> present_family;
    };

    struct SwapChainInfo {
        VkSurfaceCapabilitiesKHR surface_capabilities{};
        std::vector<VkSurfaceFormatKHR> surface_formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    struct PhysicalDeviceConfig {
        VkInstance instance = nullptr;
        VkSurfaceKHR surface = nullptr;
    };

    struct PhysicalDevice {
        PhysicalDeviceConfig config{};
        VkPhysicalDevice physical_device = nullptr;
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkExtensionProperties> extensions{};
        QueueFamilyIndices queue_family_indices{};
        SwapChainInfo swap_chain_info{};
        VkFormat depth_format = VK_FORMAT_UNDEFINED;

        operator VkPhysicalDevice() const {
            return physical_device;
        }

        VkPhysicalDevice vk() const {
            return physical_device;
        }

        VkPhysicalDevice* vk_ptr() {
            return &physical_device;
        }
    };

    PhysicalDevice pick_vulkan_physical_device(const PhysicalDeviceConfig& config);
}
