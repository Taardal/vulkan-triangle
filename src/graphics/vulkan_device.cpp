#include "vulkan_device.h"

namespace Game {
    VkQueue get_device_queue(VkDevice device, uint32_t queue_family_index) {
        constexpr uint32_t queue_index = 0;
        VkQueue queue;
        vkGetDeviceQueue(device, queue_family_index, queue_index, &queue);
        return queue;
    }

    void create_vulkan_device(Vulkan& vulkan, const DeviceConfig& config) {
        const QueueFamilyIndices& queue_family_indices = vulkan.physical_device_queue_family_indices;
        const VkPhysicalDeviceFeatures& enabled_features = vulkan.physical_device_features;
        const std::vector<VkExtensionProperties>& enabled_extensions = vulkan.physical_device_extensions;

        std::vector<const char*> enabled_extension_names;
        enabled_extension_names.reserve(enabled_extensions.size());
        for (const VkExtensionProperties& extension : enabled_extensions) {
            enabled_extension_names.push_back(extension.extensionName);
        }

        float queue_priority = 1.0f;
        std::set<u32> queue_families = {
            queue_family_indices.graphics_family.value(),
            queue_family_indices.present_family.value()
        };
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        queue_create_infos.reserve(queue_families.size());
        for (u32 queue_family : queue_families) {
            VkDeviceQueueCreateInfo queue_create_info{};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queue_family;
            queue_create_info.queueCount = 1;
            queue_create_info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(queue_create_info);
        }

        VkDeviceCreateInfo device_create_info{};
        device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pEnabledFeatures = &enabled_features;
        device_create_info.enabledExtensionCount = (u32) enabled_extension_names.size();
        device_create_info.ppEnabledExtensionNames = enabled_extension_names.data();
        device_create_info.pQueueCreateInfos = queue_create_infos.data();
        device_create_info.queueCreateInfoCount = (u32) queue_create_infos.size();

        if (vkCreateDevice(vulkan.physical_device, &device_create_info, GM_VK_ALLOCATOR, &vulkan.device) != VK_SUCCESS) {
            GM_THROW("Could not create Vulkan device");
        }

        set_vulkan_object_name(vulkan.device, vulkan.device, VK_OBJECT_TYPE_DEVICE, config.name.c_str());

        vulkan.graphicsQueue = get_device_queue(vulkan.device, queue_family_indices.graphics_family.value());
        if (!vulkan.graphicsQueue) {
            GM_THROW("Could not get Vulkan device graphics queue");
        }

        vulkan.presentQueue = get_device_queue(vulkan.device, queue_family_indices.present_family.value());
        if (!vulkan.presentQueue) {
            GM_THROW("Could not get Vulkan device present queue");
        }
    }

    void destroy_vulkan_device(const Vulkan& vulkan) {
        vkDestroyDevice(vulkan.device, GM_VK_ALLOCATOR);
    }
}
