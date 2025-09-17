#pragma once

#include <vulkan/vulkan.h>

namespace dd {
    // Forward declaration
    struct vulkan;

    struct queue_family_indices {
        std::optional<u32> graphics_family;
        std::optional<u32> present_family;
    };

    struct swap_chain_info {
        VkSurfaceCapabilitiesKHR surface_capabilities{};
        std::vector<VkSurfaceFormatKHR> surface_formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    struct physical_device {
        VkPhysicalDevice vk_physical_device = nullptr;
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkExtensionProperties> extensions{};
        queue_family_indices queue_family_indices{};
        swap_chain_info swap_chain_info{};
        VkFormat depth_format = VK_FORMAT_UNDEFINED;

        operator VkPhysicalDevice() const;
    };

    void pick_physical_device(vulkan& vulkan);

    std::vector<VkPhysicalDevice> get_physical_devices(vulkan& vulkan);

    physical_device get_most_suitable_physical_device(
        vulkan& vulkan,
        const std::vector<VkPhysicalDevice>& vk_physical_devices,
        const std::vector<const char*>& required_extensions
    );

    physical_device find_physical_device_info(
        vulkan& vulkan,
        VkPhysicalDevice vk_physical_device,
        const std::vector<const char*>& required_extensions
    );

    std::vector<VkExtensionProperties> find_physical_device_extensions(
        VkPhysicalDevice physical_device,
        const std::vector<const char*>& required_extensions
    );

    queue_family_indices find_queue_family_indices(vulkan& vulkan, VkPhysicalDevice physical_device);

    swap_chain_info find_swap_chain_info(vulkan& vulkan, VkPhysicalDevice physical_device);

    VkFormat find_depth_format(VkPhysicalDevice physical_device);

    uint32_t get_suitability_rating(
        const physical_device& physical_device,
        const std::vector<const char*>& required_extensions
    );

    bool has_required_extensions(
        const std::vector<const char*>& extensions,
        const std::vector<VkExtensionProperties>& available_extensions
    );

    bool has_required_queue_family_indices(const queue_family_indices& queue_family_indices);

    bool has_required_swap_chain_support(const swap_chain_info& swap_chain_info);

    bool has_required_features(const VkPhysicalDeviceFeatures& available_device_features);
}
