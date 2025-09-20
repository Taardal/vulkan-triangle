#include "vulkan.h"

#include "vulkan_device.h"
#include "vulkan_instance.h"
#include "vulkan_physical_device.h"
#include "vulkan_surface.h"
#include "vulkan_swap_chain.h"

namespace Game {
    void create_vulkan(Vulkan& vulkan, const VulkanConfig& config) {
        if (glfwVulkanSupported() != GLFW_TRUE) {
            GM_THROW("Vulkan is not supported");
        }

        create_vulkan_instance(vulkan, {
            .application_name = config.application_name,
            .engine_name = config.engine_name,
            .validation_layers_enabled = config.validation_layers_enabled,
        });

        create_vulkan_surface(vulkan, {
            .window = config.window,
        });

        pick_vulkan_physical_device(vulkan);

        create_vulkan_device(vulkan);

        create_vulkan_swap_chain(vulkan, {
            .window = config.window,
        });
    }

    void destroy_vulkan(const Vulkan& vulkan) {
        destroy_vulkan_swap_chain(vulkan);
        destroy_vulkan_device(vulkan);
        destroy_vulkan_surface(vulkan);
        destroy_vulkan_instance(vulkan);
    }
}
