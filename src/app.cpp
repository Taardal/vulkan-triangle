#include "app.h"

namespace Game {
    void create_app(App& app, const AppConfig& config) {
        create_window(app.window, {
            .title = config.title,
            .width = config.width,
            .height = config.height,
            .maximized = config.maximized,
            .resizable = config.resizable,
        });
        create_vulkan(app.vulkan, {
            .window = &app.window,
            .application_name = config.title,
            .engine_name = std::format("{} Engine", config.title),
            .validation_layers_enabled = true,
            .debug_extension_enabled = true,
        });
    }

    void destroy_app(const App& app) {
        destroy_vulkan(app.vulkan);
        destroy_window(app.window);
    }

    void start(App& app) {
        app.running = true;
    }

    void stop(App& app) {
        app.running = false;
    }
}
