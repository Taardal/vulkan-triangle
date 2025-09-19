#include "app.h"

namespace Game {
    App create_app(const AppConfig& config) {
        App app{};

        app.window = create_window({
            .title = config.title,
            .width = config.width,
            .height = config.height,
            .maximized = config.maximized,
            .resizable = true,
        });

        app.vulkan = create_vulkan({
            .application_name = app.window.config.title,
            .engine_name = app.window.config.title,
            .validation_layers_enabled = true,
            .window = &app.window,
        });

        return app;
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
