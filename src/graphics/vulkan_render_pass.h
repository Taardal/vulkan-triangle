#pragma once

#include "vulkan.h"

namespace Game {
    void create_vulkan_render_pass(Vulkan& vulkan);

    void destroy_vulkan_render_pass(const Vulkan& vulkan);
}
