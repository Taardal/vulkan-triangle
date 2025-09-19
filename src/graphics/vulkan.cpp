#include "vulkan.h"

#include "vulkan_physical_device.h"
#include "vulkan_device.h"
#include "vulkan_instance.h"

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
            .vulkan_instance = &vulkan.instance,
        });

        vulkan.device = create_vulkan_device({
            .physical_device = &vulkan.physical_device,
        });

        return vulkan;
    }

    void destroy_vulkan(const Vulkan& vulkan) {
        destroy_vulkan_device(vulkan.device);
        destroy_vulkan_instance(vulkan.instance);
    }
}
