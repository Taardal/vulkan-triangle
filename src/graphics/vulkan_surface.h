#pragma once

#include "vulkan.h"
#include "window/window.h"

namespace Game {
    struct SurfaceConfig {
        Window* window = nullptr;
    };

    void create_vulkan_surface(Vulkan& vulkan, const SurfaceConfig& config);

    void destroy_vulkan_surface(const Vulkan& vulkan);
}
