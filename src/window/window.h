#pragma once

#include "window/event.h"

namespace Game {
    struct WindowConfig {
        std::string title = "App";
        i32 width = 800;
        i32 height = 600;
        bool maximized = false;
        bool resizable = true;
        std::function<void(Event&)> on_event;
    };

    struct Window {
        WindowConfig config;
        GLFWwindow* glfw_window = nullptr;

        operator GLFWwindow*() const {
            return glfw_window;
        }
    };

    Window create_window(const WindowConfig& config);

    void destroy_window(const Window& window);

    void on_glfw_error(i32 error, const char* description);

    void on_glfw_key_change_event(GLFWwindow* glfwWindow, i32 key, i32 scanCode, i32 action, i32 mods);

    void on_glfw_window_close_event(GLFWwindow* glfwWindow);

    void on_glfw_event(Event& event, GLFWwindow* glfwWindow);
}
