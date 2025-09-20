#pragma once

#include "vulkan.h"

namespace Game {
    struct GraphicsPipelineConfig {
        std::filesystem::path vertex_shader_path;
        std::filesystem::path fragment_shader_path;
    };

    void create_vulkan_pipeline(Vulkan& vulkan, const GraphicsPipelineConfig& config);

    void destroy_vulkan_pipeline(const Vulkan& vulkan);
}
