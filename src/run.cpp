#include "run.h"
#include "game_loop.h"
#include "window/key_event.h"

namespace Game {
    void update(App& app, f64 timestep) {
    }

    void render(App& app) {
        render_frame(app.vulkan);
    }

    void on_event(App& app, Event& e) {
        if (e.type == EventType::WindowClose) {
            stop(app);
        }
        if (e.type == EventType::KeyPressed) {
            auto& event = (KeyPressedEvent&) e;
            if (event.key == Key::Escape) {
                stop(app);
            }
        }
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
        try {
            App app{};
            create_app(app, config);
            init(app);
            start(app);
            game_loop(app, {
                .on_update = update,
                .on_render = render,
            });
            destroy_app(app);
        } catch (const Game::Error& e) {
            GM_LOG_CRITICAL("Fatal error");
            e.printStacktrace();
        } catch (const std::exception& e) {
            GM_LOG_CRITICAL("Fatal error: {}", e.what());
        }
    }
}