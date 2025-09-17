#include "physical_device.h"
#include "vulkan.h"

namespace Game {
    physical_device::operator VkPhysicalDevice() const {
        return vk_physical_device;
    }

    void pick_physical_device(Vulkan& vulkan) {
        GM_ASSERT(vulkan.instance != nullptr, "Vulkan instance must be created before picking a physical device");
        GM_ASSERT(vulkan.surface != nullptr, "Vulkan surface must be created before picking a physical device");

        std::vector<VkPhysicalDevice> available_devices = get_physical_devices(vulkan);
        GM_ASSERT(!available_devices.empty(), "System must have a physical device");

        std::vector<const char*> required_extensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        #ifdef GM_PLATFORM_MACOS
            required_extensions.push_back("VK_KHR_portability_subset");
        #endif
        physical_device most_suitable_device = get_most_suitable_physical_device(vulkan, available_devices, required_extensions);
        GM_ASSERT(most_suitable_device.vk_physical_device != nullptr, "Could not find any suitable physical device");

        vulkan.physical_device = most_suitable_device;
        std::cout << "Picked physical device [" << most_suitable_device.properties.deviceName << "]" << std::endl;
    }

    std::vector<VkPhysicalDevice> get_physical_devices(Vulkan& vulkan) {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(vulkan.instance, &device_count, nullptr);
        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(vulkan.instance, &device_count, devices.data());
        return devices;
    }

    physical_device get_most_suitable_physical_device(
        Vulkan& vulkan,
        const std::vector<VkPhysicalDevice>& vk_physical_devices,
        const std::vector<const char*>& required_extensions
    ) {
        std::multimap<uint32_t , physical_device> devices_by_rating;
        for (VkPhysicalDevice vk_physical_device : vk_physical_devices) {
            physical_device physical_device = find_physical_device_info(vulkan, vk_physical_device, required_extensions);
            uint32_t suitability_rating = get_suitability_rating(physical_device, required_extensions);
            devices_by_rating.insert(std::make_pair(suitability_rating, physical_device));
        }
        uint32_t highest_rating = devices_by_rating.rbegin()->first;
        if (highest_rating == 0) {
            return {};
        }
        return devices_by_rating.rbegin()->second;
    }

    physical_device find_physical_device_info(
        Vulkan& vulkan,
        VkPhysicalDevice vk_physical_device,
        const std::vector<const char*>& required_extensions
    ) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(vk_physical_device, &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(vk_physical_device, &features);

        physical_device physical_device{};
        physical_device.vk_physical_device = vk_physical_device;
        physical_device.properties = properties;
        physical_device.features = features;
        physical_device.extensions = find_physical_device_extensions(vk_physical_device, required_extensions);
        physical_device.queue_family_indices = find_queue_family_indices(vulkan, vk_physical_device);
        physical_device.swap_chain_info = find_swap_chain_info(vulkan, vk_physical_device);
        physical_device.depth_format = find_depth_format(vk_physical_device);
        return physical_device;
    }

    std::vector<VkExtensionProperties> find_physical_device_extensions(
        VkPhysicalDevice physical_device,
        const std::vector<const char*>& required_extensions
    ) {
        const char* layer_name = nullptr;
        uint32_t extension_count = 0;
        vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, nullptr);
        std::vector<VkExtensionProperties> available_extensions(extension_count);
        vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &extension_count, available_extensions.data());

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

    queue_family_indices find_queue_family_indices(Vulkan& vulkan, VkPhysicalDevice physical_device) {
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

        queue_family_indices indices;
        for (u32 queue_family_index = 0; queue_family_index < queue_families.size(); queue_family_index++) {
            const VkQueueFamilyProperties& queue_family = queue_families[queue_family_index];
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = queue_family_index;
            }
            VkBool32 presentation_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queue_family_index, vulkan.surface, &presentation_support);
            if (presentation_support) {
                indices.present_family = queue_family_index;
            }
            if (has_required_queue_family_indices(indices)) {
                break;
            }
        }
        return indices;
    }

    swap_chain_info find_swap_chain_info(Vulkan& vulkan, VkPhysicalDevice physical_device) {
        swap_chain_info swap_chain_info;

        VkSurfaceKHR surface = vulkan.surface;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &swap_chain_info.surface_capabilities);

        uint32_t format_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
        swap_chain_info.surface_formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, swap_chain_info.surface_formats.data());

        uint32_t presentation_mode_count = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentation_mode_count, nullptr);
        swap_chain_info.present_modes.resize(presentation_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentation_mode_count, swap_chain_info.present_modes.data());

        return swap_chain_info;
    }

    VkFormat find_depth_format(VkPhysicalDevice physical_device) {
        std::vector<VkFormat> depth_formats = {
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

    uint32_t get_suitability_rating(
        const physical_device& physical_device,
        const std::vector<const char*>& required_extensions
    ) {
        if (!has_required_features(physical_device.features)) {
            GM_LOG_DEBUG("[{}] does not have required device features", physical_device.properties.deviceName);
            return 0;
        }
        if (!has_required_extensions(required_extensions, physical_device.extensions)) {
            GM_LOG_DEBUG("[{}] does not have required device extensions", physical_device.properties.deviceName);
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
        uint32_t rating = physical_device.properties.limits.maxImageDimension2D;
        if (physical_device.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            rating += 1000;
        }
        return rating;
    }

    bool has_required_extensions(
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

    bool has_required_queue_family_indices(const queue_family_indices& queue_family_indices) {
        return queue_family_indices.graphics_family.has_value() && queue_family_indices.present_family.has_value();
    }

    bool has_required_swap_chain_support(const swap_chain_info& swap_chain_info) {
        return !swap_chain_info.surface_formats.empty() && !swap_chain_info.present_modes.empty();
    }

    bool has_required_features(const VkPhysicalDeviceFeatures& available_device_features) {
        return available_device_features.samplerAnisotropy;
    }
}
