#pragma once

#include "graphics/vulkan.h"
#include "window/window.h"

namespace Game {
    struct RendererConfig {
        Window* window = nullptr;
        std::string app_name = "";
        bool debug_enabled = false;
        u32 max_frames_in_flight = 0;
    };

    struct Renderer {
        Vulkan vulkan{};
        u32 current_frame = 0;
        u32 max_frames_in_flight = 0;
    };

    void create_renderer(Renderer& renderer, const RendererConfig& config);

    void destroy_renderer(const Renderer& renderer);

    void render_frame(Renderer& renderer);
}
