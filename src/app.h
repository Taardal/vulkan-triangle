#pragma once

#include "graphics/renderer.h"
#include "window/window.h"

namespace Game {
    struct AppConfig {
        std::string title = "Game";
        i32 width = 800;
        i32 height = 600;
        bool maximized = false;
        bool resizable = true;
        LogLevel log_level = LogLevel::trace;
    };

    struct App {
        AppConfig config{};
        bool running = false;
        Renderer renderer{};
        Window window{};
    };

    void create_app(App& app, const AppConfig& config);

    void destroy_app(const App& app);

    void start(App& app);

    void stop(App& app);
}
