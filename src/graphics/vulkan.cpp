#include "vulkan.h"

namespace Game {
    Vulkan create_vulkan(const VulkanConfig& config) {
        if (glfwVulkanSupported() != GLFW_TRUE) {
            GM_THROW("Vulkan is not supported");
        }

        Vulkan vulkan{};
        vulkan.config = config;

        vulkan.instance = create_vulkan_instance({
            .application_name = config.application_name,
            .engine_name = config.engine_name,
            .validation_layers_enabled = config.validation_layers_enabled,
            .window = config.window,
        });

        vulkan.physical_device = pick_vulkan_physical_device({
            .instance = vulkan.instance,
            .surface = vulkan.instance.surface,
        });

        vulkan.device = create_vulkan_device({
            .physical_device = vulkan.physical_device,
            .physical_device_features = vulkan.physical_device.features,
            .physical_device_extensions = vulkan.physical_device.extensions,
            .queue_family_indices = vulkan.physical_device.queue_family_indices,
        });

        vulkan.swap_chain = create_vulkan_swap_chain({
            .window = config.window,
            .device = vulkan.device,
            .surface = vulkan.instance.surface,
            .swap_chain_info = vulkan.physical_device.swap_chain_info,
            .queue_family_indices = vulkan.physical_device.queue_family_indices,
        });

        return vulkan;
    }

    void destroy_vulkan(const Vulkan& vulkan) {
        destroy_vulkan_swap_chain(vulkan.swap_chain);
        destroy_vulkan_device(vulkan.device);
        destroy_vulkan_instance(vulkan.instance);
    }
}
