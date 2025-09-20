#pragma once

#include "window/event.h"

namespace Game {
    struct WindowConfig {
        std::string title = "App";
        i32 width = 800;
        i32 height = 600;
        bool maximized = false;
        bool resizable = true;
    };

    struct Window {
        GLFWwindow* glfw_window = nullptr;
        std::function<void(Event&)> on_event;

        operator GLFWwindow*() const {
            return glfw_window;
        }
    };

    void create_window(Window& window, const WindowConfig& config);

    void destroy_window(const Window& window);

    void set_window_event_listener(Window& window, const std::function<void(Event&)>& on_event);

    struct WindowSize {
        i32 width = 0;
        i32 height = 0;
    };

    WindowSize get_window_size(const Window& window);
}
