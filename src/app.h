#pragma once

#include "graphics/vulkan.h"
#include "system/clock.h"
#include "window/window.h"
#include "window/event.h"

namespace dd {
    struct app_config {
        std::string name = "App";
        int32_t window_width = 800;
        int32_t window_height = 600;
        bool window_maximized = false;
        bool window_resizable = false;
    };

    struct app {
       app_config config;
       window window;
       vulkan vulkan;
       clock clock;
       bool initialized = false;
       bool running = false;
       bool paused = false;

       explicit app(app_config config);
    };

    void run(app& app);

    void initialize(app& app);

    void terminate(app& app);

    void game_loop(app& app);

    void process_event(app& app, Event& event);

    void update(app& app, double timestep);

    void render();
}
