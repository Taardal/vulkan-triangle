#include "app.h"
#include "spdlog/common.h"
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "window/event.h"
#include "window/key_event.h"

namespace dd {
    void run_app(const app_config& config) {
        app app({
            .config = std::move(config),
        });
        try {
            initialize_app(app);
            app.initialized = true;
        } catch (const std::exception& e) {
            std::cerr << "Initialization error: " << e.what() << std::endl;
            app.initialized = false;
        }
        if (app.initialized) {
            try {
                app.running = true;
                game_loop(app);
            } catch (const std::exception& e) {
                std::cerr << "Runtime error: " << e.what() << std::endl;
                app.running = false;
            }
        }
        terminate_app(app);
    }

    void initialize_app(app& app) {
        app.window = new window({
            .title = app.config.name,
            .width = app.config.window_width,
            .height = app.config.window_height,
            .maximized = app.config.window_maximized,
            .resizable = app.config.window_resizable,
            .on_event = [&app](Event& event) {
                process_event(app, event);
            },
        });
        std::cout << "Created window" << std::endl;
    }

    void terminate_app(app& app) {
        delete app.window;
        std::cout << "Destroyed window" << std::endl;
    }

    void game_loop(app& app) {
        constexpr double one_second = 1.0;
        double last_uptime_seconds = 0.0;
        start_clock(app.clock);
        while(app.running) {
            double uptime_seconds = get_time<sec>(app.clock);
            double timestep = std::min(uptime_seconds - last_uptime_seconds, one_second);
            last_uptime_seconds = uptime_seconds;
            poll_events();
            if (!app.paused) {
                update(app, timestep);
            }
            render();
        }
    }

    void process_event(app& app, Event& event) {
        std::cout << "Processing event [" << event << "]" << std::endl;
        if (event.type == EventType::KeyPressed && event.as<KeyPressedEvent>().key == Key::Escape) {
           app.running = false;
        }
    }

    void update(app& app, double timestep) {
    }

    void render() {
    }
}
