#pragma once

#include "vulkan.h"

namespace Game {
    struct PipelineConfig {
        std::string name = "Pipeline";
        std::filesystem::path vertex_shader_path;
        std::filesystem::path fragment_shader_path;
    };

    void create_vulkan_pipeline(Vulkan& vulkan, const PipelineConfig& config);

    void destroy_vulkan_pipeline(const Vulkan& vulkan);
}
