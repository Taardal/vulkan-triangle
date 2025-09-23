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
        create_renderer(app.renderer, {
            .window = &app.window,
            .app_name = config.title,
            .debug_enabled = true,
            .max_frames_in_flight = 2,
        });
    }

    void destroy_app(const App& app) {
        destroy_renderer(app.renderer);
        destroy_window(app.window);
    }

    void start(App& app) {
        app.running = true;
    }

    void stop(App& app) {
        app.running = false;
    }
}
