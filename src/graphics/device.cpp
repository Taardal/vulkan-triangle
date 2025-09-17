#include "device.h"
#include "vulkan.h"

namespace Game {
    VulkanDevice::operator VkDevice() const {
        return device;
    }

    VkDevice* VulkanDevice::vk_ptr() {
        return &device;
    }
}

namespace Game {
    void create_device(Vulkan& vulkan) {
        const queue_family_indices& queue_family_indices = vulkan.physical_device.queue_family_indices;
        const VkPhysicalDeviceFeatures& enabled_features = vulkan.physical_device.features;
        const std::vector<VkExtensionProperties>& enabled_extensions = vulkan.physical_device.extensions;

        std::vector<const char*> enabled_extension_names;
        //enabled_extension_names.resize(enabled_extensions.size());
        for (const VkExtensionProperties& extension : enabled_extensions) {
            enabled_extension_names.push_back(extension.extensionName);
        }

        // Vulkan lets you assign priorities to queues to influence the scheduling of command buffer execution using floating point numbers between 0.0 and 1.0.
        // This is required even if there is only a single queue
        float queue_priority = 1.0f;
        std::set<u32> queue_families = {
            queue_family_indices.graphics_family.value(),
            queue_family_indices.present_family.value()
        };
        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        //queue_create_infos.resize(queue_families.size());
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

        GM_ASSERT_VK_SUCCESS(
            vkCreateDevice(vulkan.physical_device, &device_create_info, GM_VK_ALLOCATOR, vulkan.device.vk_ptr()),
            "Could not create device"
        );

        vulkan.device.graphicsQueue = get_device_queue(vulkan, queue_family_indices.graphics_family.value());
        GM_ASSERT(vulkan.device.graphicsQueue != nullptr, "Could not get graphics device queue");

        vulkan.device.presentQueue = get_device_queue(vulkan, queue_family_indices.present_family.value());
        GM_ASSERT(vulkan.device.presentQueue != nullptr, "Could not get present device queue");
    }

    void destroy_device(Vulkan& vulkan) {
        vkDestroyDevice(vulkan.device, GM_VK_ALLOCATOR);
    }

    VkQueue get_device_queue(Vulkan& vulkan, uint32_t queue_family_index) {
        constexpr uint32_t queue_index = 0;
        VkQueue queue;
        vkGetDeviceQueue(vulkan.device, queue_family_index, queue_index, &queue);
        return queue;
    }
}
