#include "vulkan_physical_device.h"

namespace Game {
    struct PhysicalDeviceInfo {
        VkPhysicalDevice physical_device = nullptr;
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkExtensionProperties> extensions{};
        VkSurfaceCapabilitiesKHR surface_capabilities{};
        std::vector<VkSurfaceFormatKHR> surface_formats;
        std::vector<VkPresentModeKHR> present_modes;
        QueueFamilyIndices queue_family_indices{};
        VkFormat depth_format = VK_FORMAT_UNDEFINED;
    };

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
        if (vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, nullptr) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan physical device extension count");
        }
        std::vector<VkExtensionProperties> extensions(extension_count);
        if (vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, extensions.data()) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan physical device extensions");
        }
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

    bool has_required_swap_chain_support(
        const std::vector<VkSurfaceFormatKHR>& surface_formats,
        const std::vector<VkPresentModeKHR>& present_modes
    ) {
        return !surface_formats.empty() && !present_modes.empty();
    }

    bool has_required_features(const VkPhysicalDeviceFeatures& available_device_features) {
        return available_device_features.samplerAnisotropy;
    }

    u32 get_suitability_rating(
        const PhysicalDeviceInfo& physical_device_info,
        const std::vector<const char*>& required_extensions
    ) {
        if (!has_extensions(required_extensions, physical_device_info.extensions)) {
            GM_LOG_DEBUG("[{}] does not have required device extensions", physical_device_info.properties.deviceName);
            return 0;
        }
        if (!has_required_features(physical_device_info.features)) {
            GM_LOG_DEBUG("[{}] does not have required device features", physical_device_info.properties.deviceName);
            return 0;
        }
        if (!has_required_queue_family_indices(physical_device_info.queue_family_indices)) {
            GM_LOG_DEBUG("[{}] does not have required queue family indices", physical_device_info.properties.deviceName);
            return 0;
        }
        if (!has_required_swap_chain_support(physical_device_info.surface_formats, physical_device_info.present_modes)) {
            GM_LOG_DEBUG("[{}] does not have required swap chain info", physical_device_info.properties.deviceName);
            return 0;
        }
        if (physical_device_info.depth_format == VK_FORMAT_UNDEFINED) {
            GM_LOG_DEBUG("[{}] does not have a suitable depth format", physical_device_info.properties.deviceName);
            return 0;
        }
        u32 rating = physical_device_info.properties.limits.maxImageDimension2D;
        if (physical_device_info.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
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

    VkSurfaceCapabilitiesKHR get_surface_capabilities(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
        VkSurfaceCapabilitiesKHR surface_capabilities{};
        if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan physical device surface capabilities");
        }
        return surface_capabilities;
    }

    std::vector<VkSurfaceFormatKHR> get_surface_formats(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
        u32 format_count = 0;
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan physical device surface format count");
        }
        std::vector<VkSurfaceFormatKHR> surface_formats(format_count);
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, surface_formats.data()) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan physical device surface formats");
        }
        return surface_formats;
    }

    std::vector<VkPresentModeKHR> get_present_modes(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
        u32 present_mode_count = 0;
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan physical device present mode count");
        }
        std::vector<VkPresentModeKHR> present_modes(present_mode_count);
        if (vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes.data()) != VK_SUCCESS) {
            GM_THROW("Could not get Vulkan physical device present modes");
        }
        return present_modes;
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

    VkPhysicalDeviceProperties get_properties(VkPhysicalDevice physical_device) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_device, &properties);
        return properties;
    }

    VkPhysicalDeviceFeatures get_features(VkPhysicalDevice physical_device) {
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physical_device, &features);
        return features;
    }

    PhysicalDeviceInfo get_physical_device_info(
        const Vulkan& vulkan,
        VkPhysicalDevice physical_device,
        const std::vector<const char*>& required_extensions
    ) {
        PhysicalDeviceInfo physical_device_info{};
        physical_device_info.physical_device = physical_device;
        physical_device_info.properties = get_properties(physical_device);
        physical_device_info.features = get_features(physical_device);
        physical_device_info.extensions = get_extensions(physical_device, required_extensions);
        physical_device_info.queue_family_indices = get_queue_family_indices(physical_device, vulkan.surface);
        physical_device_info.surface_capabilities = get_surface_capabilities(physical_device, vulkan.surface);
        physical_device_info.surface_formats = get_surface_formats(physical_device, vulkan.surface);
        physical_device_info.present_modes = get_present_modes(physical_device, vulkan.surface);
        physical_device_info.depth_format = get_depth_format(physical_device);
        return physical_device_info;
    }

    PhysicalDeviceInfo get_most_suitable_physical_device(
        const Vulkan& vulkan,
        const std::vector<VkPhysicalDevice>& available_devices,
        const std::vector<const char*>& required_extensions
    ) {
        std::multimap<u32 , PhysicalDeviceInfo> devices_by_rating;
        for (VkPhysicalDevice available_device : available_devices) {
            PhysicalDeviceInfo physical_device_info = get_physical_device_info(vulkan, available_device, required_extensions);
            u32 suitability_rating = get_suitability_rating(physical_device_info, required_extensions);
            devices_by_rating.insert(std::make_pair(suitability_rating, physical_device_info));
        }
        u32 highest_rating = devices_by_rating.rbegin()->first;
        if (highest_rating == 0) {
            return {};
        }
        return devices_by_rating.rbegin()->second;
    }

    std::vector<VkPhysicalDevice> get_available_physical_devices(const Vulkan& vulkan) {
        u32 device_count = 0;
        if (vkEnumeratePhysicalDevices(vulkan.instance, &device_count, nullptr) != VK_SUCCESS) {
            GM_THROW("Could not get physical device count");
        }
        std::vector<VkPhysicalDevice> devices(device_count);
        if (vkEnumeratePhysicalDevices(vulkan.instance, &device_count, devices.data()) != VK_SUCCESS) {
            GM_THROW("Could not get physical devices");
        }
        return devices;
    }

    void pick_vulkan_physical_device(Vulkan& vulkan) {
        std::vector<VkPhysicalDevice> available_devices = get_available_physical_devices(vulkan);
        if (available_devices.empty()) {
            GM_THROW("Could not find any available physical device");
        }

        PhysicalDeviceInfo most_suitable_device_info = get_most_suitable_physical_device(vulkan, available_devices, get_required_extensions());
        if (!most_suitable_device_info.physical_device) {
            GM_THROW("Could not find any suitable physical device");
        }

        GM_LOG_DEBUG("Picked physical device [{}]", most_suitable_device_info.properties.deviceName);

        vulkan.physical_device = most_suitable_device_info.physical_device;
        vulkan.physical_device_properties = most_suitable_device_info.properties;
        vulkan.physical_device_features = most_suitable_device_info.features;
        vulkan.physical_device_extensions = most_suitable_device_info.extensions;
        vulkan.physical_device_surface_capabilities = most_suitable_device_info.surface_capabilities;
        vulkan.physical_device_surface_formats = most_suitable_device_info.surface_formats;
        vulkan.physical_device_present_modes = most_suitable_device_info.present_modes;
        vulkan.physical_device_queue_family_indices = most_suitable_device_info.queue_family_indices;
        vulkan.physical_device_depth_format = most_suitable_device_info.depth_format;
    }
}
