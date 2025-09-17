#include "app.h"
#include "graphics/vulkan.h"
#include "window/key_event.h"
#include "window/window.h"

namespace Game {
    static bool running = true;

    void on_event(Event& e) {
        if (e.type == EventType::WindowClose) {
            running = false;
        }
        if (e.type == EventType::KeyPressed) {
            auto& event = (KeyPressedEvent&) e;
            if (event.key == Key::Escape) {
                running = false;
            }
        }
    }

    void run() {
        Window window = create_window({
            .title = "App",
            .width = 1280,
            .height = 768,
            .maximized = false,
            .resizable = true,
            .on_event = &on_event,
        });

        Vulkan vulkan = create_vulkan({
            .application_name = window.config.title,
            .engine_name = window.config.title,
            .validation_layers_enabled = true,
            .window = &window,
        });

        while (running) {
            glfwPollEvents();
        }

        destroy_vulkan(vulkan);
        destroy_window(window);
    }
}
