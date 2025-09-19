#include "vulkan_physical_device.h"

namespace Game {
    std::vector<const char*> get_required_extensions() {
        std::vector extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
#ifdef GM_PLATFORM_MACOS
        extensions.push_back("VK_KHR_portability_subset");
#endif
        return extensions;
    }

    std::vector<VkExtensionProperties> get_available_extensions(VkPhysicalDevice physical_device) {
        const char* layer_name = nullptr;
        u32 extension_count = 0;
        vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, nullptr);
        std::vector<VkExtensionProperties> extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, extensions.data());
        return extensions;
    }

    std::vector<VkExtensionProperties> get_extensions(
        VkPhysicalDevice physical_device,
        const std::vector<const char*>& required_extensions
    ) {
        auto available_extensions = get_available_extensions(physical_device);

        std::vector<VkExtensionProperties> extensions;
        for (const char* required_extension : required_extensions) {
            for (const VkExtensionProperties& available_extension : available_extensions) {
                if (strcmp(required_extension, available_extension.extensionName) == 0) {
                    extensions.push_back(available_extension);
                }
            }
        }
        return extensions;
    }

    bool has_extensions(
        const std::vector<const char*>& extensions,
        const std::vector<VkExtensionProperties>& available_extensions
    ) {
        for (const char* extension: extensions) {
            bool extension_found = false;
            for (const VkExtensionProperties& available_extension: available_extensions) {
                if (strcmp(extension, available_extension.extensionName) == 0) {
                    extension_found = true;
                    break;
                }
            }
            if (!extension_found) {
                GM_LOG_ERROR("Could not find extension [{0}]", extension);
                return false;
            }
        }
        return true;
    }

    bool has_required_queue_family_indices(const QueueFamilyIndices& queue_family_indices) {
        return queue_family_indices.graphics_family.has_value() && queue_family_indices.present_family.has_value();
    }

    bool has_required_swap_chain_support(const SwapChainInfo& swap_chain_info) {
        return !swap_chain_info.surface_formats.empty() && !swap_chain_info.present_modes.empty();
    }

    bool has_required_features(const VkPhysicalDeviceFeatures& available_device_features) {
        return available_device_features.samplerAnisotropy;
    }

    u32 get_suitability_rating(
        const PhysicalDevice& physical_device,
        const std::vector<const char*>& required_extensions
    ) {
        if (!has_extensions(required_extensions, physical_device.extensions)) {
            GM_LOG_DEBUG("[{}] does not have required device extensions", physical_device.properties.deviceName);
            return 0;
        }
        if (!has_required_features(physical_device.features)) {
            GM_LOG_DEBUG("[{}] does not have required device features", physical_device.properties.deviceName);
            return 0;
        }
        if (!has_required_queue_family_indices(physical_device.queue_family_indices)) {
            GM_LOG_DEBUG("[{}] does not have required queue family indices", physical_device.properties.deviceName);
            return 0;
        }
        if (!has_required_swap_chain_support(physical_device.swap_chain_info)) {
            GM_LOG_DEBUG("[{}] does not have required swap chain info", physical_device.properties.deviceName);
            return 0;
        }
        if (physical_device.depth_format == VK_FORMAT_UNDEFINED) {
            GM_LOG_DEBUG("[{}] does not have a suitable depth format", physical_device.properties.deviceName);
            return 0;
        }
        u32 rating = physical_device.properties.limits.maxImageDimension2D;
        if (physical_device.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            rating += 1000;
        }
        return rating;
    }

    VkFormat get_depth_format(VkPhysicalDevice physical_device) {
        std::vector depth_formats = {
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D24_UNORM_S8_UINT
        };
        VkFormatFeatureFlagBits required_features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
        for (VkFormat depth_format : depth_formats) {
            VkFormatProperties format_properties;
            vkGetPhysicalDeviceFormatProperties(physical_device, depth_format, &format_properties);
            if ((format_properties.optimalTilingFeatures & required_features) == required_features) {
                return depth_format;
            }
        }
        return VK_FORMAT_UNDEFINED;
    }

    SwapChainInfo get_swap_chain_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
        SwapChainInfo swap_chain_info{};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &swap_chain_info.surface_capabilities);

        u32 format_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
        swap_chain_info.surface_formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, swap_chain_info.surface_formats.data());

        u32 presentation_mode_count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentation_mode_count, nullptr);
        swap_chain_info.present_modes.resize(presentation_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentation_mode_count, swap_chain_info.present_modes.data());

        return swap_chain_info;
    }

    QueueFamilyIndices get_queue_family_indices(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
        u32 queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

        QueueFamilyIndices indices;
        for (u32 queue_family_index = 0; queue_family_index < queue_families.size(); queue_family_index++) {
            const VkQueueFamilyProperties& queue_family = queue_families[queue_family_index];
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = queue_family_index;
            }
            VkBool32 presentation_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queue_family_index, surface, &presentation_support);
            if (presentation_support) {
                indices.present_family = queue_family_index;
            }
            if (has_required_queue_family_indices(indices)) {
                break;
            }
        }
        return indices;
    }

    PhysicalDevice get_physical_device(
        const PhysicalDeviceConfig& config,
        VkPhysicalDevice vk_physical_device,
        const std::vector<const char*>& required_extensions
    ) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(vk_physical_device, &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(vk_physical_device, &features);

        PhysicalDevice physical_device{};
        physical_device.physical_device = vk_physical_device;
        physical_device.properties = properties;
        physical_device.features = features;
        physical_device.extensions = get_extensions(vk_physical_device, required_extensions);
        physical_device.queue_family_indices = get_queue_family_indices(vk_physical_device, config.vulkan_instance->surface);
        physical_device.swap_chain_info = get_swap_chain_info(vk_physical_device, config.vulkan_instance->surface);
        physical_device.depth_format = get_depth_format(vk_physical_device);
        return physical_device;
    }

    PhysicalDevice get_most_suitable_physical_device(
        const PhysicalDeviceConfig& config,
        const std::vector<VkPhysicalDevice>& available_devices,
        const std::vector<const char*>& required_extensions
    ) {
        std::multimap<u32 , PhysicalDevice> devices_by_rating;
        for (VkPhysicalDevice available_device : available_devices) {
            PhysicalDevice physical_device = get_physical_device(config, available_device, required_extensions);
            u32 suitability_rating = get_suitability_rating(physical_device, required_extensions);
            devices_by_rating.insert(std::make_pair(suitability_rating, physical_device));
        }
        u32 highest_rating = devices_by_rating.rbegin()->first;
        if (highest_rating == 0) {
            return {};
        }
        return devices_by_rating.rbegin()->second;
    }

    std::vector<VkPhysicalDevice> get_available_physical_devices(const PhysicalDeviceConfig& config) {
        u32 device_count = 0;
        vkEnumeratePhysicalDevices(config.vulkan_instance->instance, &device_count, nullptr);
        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(config.vulkan_instance->instance, &device_count, devices.data());
        return devices;
    }

    PhysicalDevice pick_vulkan_physical_device(const PhysicalDeviceConfig& config) {
        GM_ASSERT(config.vulkan_instance->instance, "Vulkan instance must be created before picking a physical device");
        GM_ASSERT(config.vulkan_instance->surface, "Vulkan surface must be created before picking a physical device");

        std::vector<VkPhysicalDevice> available_devices = get_available_physical_devices(config);
        if (available_devices.empty()) {
            GM_THROW("Could not find any available physical device");
        }

        PhysicalDevice most_suitable_device = get_most_suitable_physical_device(config, available_devices, get_required_extensions());
        if (!most_suitable_device.physical_device) {
            GM_THROW("Could not find any suitable physical device");
        }

        GM_LOG_DEBUG("Picked physical device [{}]", most_suitable_device.properties.deviceName);
        most_suitable_device.config = config;

        return most_suitable_device;
    }
}
