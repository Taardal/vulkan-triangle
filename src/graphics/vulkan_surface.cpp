#include "vulkan_surface.h"

namespace Game {
    void create_vulkan_surface(Vulkan& vulkan, const SurfaceConfig& config) {
        if (glfwCreateWindowSurface(vulkan.instance, config.window->glfw_window, GM_VK_ALLOCATOR, &vulkan.surface) != VK_SUCCESS) {
            GM_THROW("Could not create Vulkan surface");
        }
    }

    void destroy_vulkan_surface(const Vulkan& vulkan) {
        if (vulkan.surface) {
            vkDestroySurfaceKHR(vulkan.instance, vulkan.surface, GM_VK_ALLOCATOR);
        }
    }
}