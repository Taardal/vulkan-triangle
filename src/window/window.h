#pragma once

#include "window/event.h"

// Vulkan must be included before GLFW
#include <vulkan/vulkan.h>
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
        GLFWwindow* glfw_window = nullptr;

        window(window_config config) : config(std::move(config)) {}
    };

    void initialize_glfw(window& window);

    void terminate_glfw(window& window);

    void poll_events();

    void on_glfw_error(int32_t error, const char* description);

    void on_glfw_key_change_event(GLFWwindow* glfwWindow, int32_t key, int32_t scanCode, int32_t action, int32_t mods);

    void on_glfw_window_close_event(GLFWwindow* glfwWindow);

    void on_glfw_event(Event& event, GLFWwindow* glfwWindow);
}
