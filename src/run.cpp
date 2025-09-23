#include "run.h"
#include "game_loop.h"
#include "window/key_event.h"

namespace Game {
    void on_event(App& app, Event& e) {
        if (e.type == EventType::WindowClose) {
            stop_app(app);
            return;
        }
        if (e.type == EventType::KeyPressed) {
            auto& event = (KeyPressedEvent&) e;
            if (event.key == Key::Escape) {
                stop_app(app);
                return;
            }
        }
        handle_renderer_event(app.renderer, e);
    }

    void update(App& app, f64 timestep) {
    }

    void render(App& app) {
        render_frame(app.renderer);
    }

    void init(App& app) {
        set_window_event_listener(app.window, [&app](Event& event) {
           on_event(app, event);
        });
    }

    void init(const AppConfig& config) {
        initialize_error_signal_handlers();
        initialize_log(config.log_level);
    }

    void run(const AppConfig& config) {
        init(config);

        App app{};
        create_app(app, config);
        init(app);

        start_app(app);
        run_game_loop(app, {
            .on_update = update,
            .on_render = render,
        });

        destroy_app(app);
    }
}