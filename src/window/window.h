#pragma once

#include "window/event.h"

#include <GLFW/glfw3.h>

namespace dd {
    struct window_config {
        std::string title;
        i32 width;
        i32 height;
        bool maximized;
        bool resizable;
        std::function<void(Event&)> on_event;
    };

    struct window {
        window_config config;
        GLFWwindow* glfwWindow = nullptr;

        window(const window_config& config);

        ~window();
    };

    void poll_events();

    void on_error(int32_t error, const char* description);

    void on_key(GLFWwindow* glfwWindow, int32_t key, int32_t scanCode, int32_t action, int32_t mods);

    void on_window_close(GLFWwindow* glfwWindow);

    void send_window_event(Event& event, GLFWwindow* glfwWindow);
}
