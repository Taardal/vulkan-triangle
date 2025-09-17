#include "app.h"
#include "window/event.h"
#include "window/key_event.h"

namespace dd {
    app::app(app_config config)
        : config(std::move(config)),
          window({
              .title = config.name,
              .width = config.window_width,
              .height = config.window_height,
              .maximized = config.window_maximized,
              .resizable = config.window_resizable,
              .on_event = [this](Event& event) {
                  process_event(*this, event);
              },
          }),
          vulkan({
              .application_name = config.name,
              .engine_name = config.name,
              .validation_layers_enabled = true,
              .window = &window,
          }) {}

    void run(app& app) {
        initializeErrorSignalHandlers();
        try {
            std::cout << "Initializing..." << std::endl;
            initialize(app);
            app.initialized = true;
        } catch (const Error& e) {
            std::cerr << "Initialization error" << std::endl;
            e.printStacktrace();
        } catch (const std::exception& e) {
            std::cerr << "Initialization error: " << e.what() << std::endl;
        }
        if (app.initialized) {
            try {
                std::cout << "Running..." << std::endl;
                app.running = true;
                game_loop(app);
            } catch (const Error& e) {
                std::cerr << "Runtime error" << std::endl;
                e.printStacktrace();
            } catch (const std::exception& e) {
                std::cerr << "Runtime error: " << e.what() << std::endl;
            }
        }
        std::cout << "Terminating..." << std::endl;
        terminate(app);
    }

    void initialize(app& app) {
        initialize_glfw(app.window);
        std::cout << "Initialized GLFW" << std::endl;

        initialize_vulkan(app.vulkan);
        std::cout << "Initialized Vulkan" << std::endl;
    }

    void terminate(app& app) {
        terminate_vulkan(app.vulkan);
        std::cout << "Terminated Vulkan" << std::endl;

        terminate_glfw(app.window);
        std::cout << "Terminated GLFW" << std::endl;
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
