#include "window.h"
#include "key_event.h"
#include "window_event.h"

#include <GLFW/glfw3.h>

namespace dd {

    window::window(const window_config& config) : config(std::move(config)) {
        DD_ASSERT_THROW(glfwInit());

        glfwSetErrorCallback(on_error);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, config.resizable);
        glfwWindowHint(GLFW_MAXIMIZED, config.maximized);

        GLFWmonitor* fullscreenMonitor = nullptr;
        GLFWwindow* sharedWindow = nullptr;
        glfwWindow = glfwCreateWindow(
            config.width,
            config.height,
            config.title.c_str(),
            fullscreenMonitor,
            sharedWindow
        );
        DD_ASSERT_THROW(glfwWindow != nullptr);

        glfwSetWindowUserPointer(glfwWindow, this);
        glfwSetKeyCallback(glfwWindow, on_key);
        glfwSetWindowCloseCallback(glfwWindow, on_window_close);
    }

    window::~window() {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    void poll_events() {
        glfwPollEvents();
    }

    void on_error(int32_t error, const char* description) {
       std::cerr << "GLFW error: [" << error << "]" << description << std::endl;
    }

    void on_key(GLFWwindow* glfwWindow, int32_t key, int32_t scanCode, int32_t action, int32_t mods) {
        if (action == GLFW_PRESS) {
            KeyPressedEvent event(key, mods, scanCode);
            send_window_event(event, glfwWindow);
        } else if (action == GLFW_RELEASE) {
            KeyReleasedEvent event(key, mods, scanCode);
            send_window_event(event, glfwWindow);
        } else if (action == GLFW_REPEAT) {
            KeyRepeatedEvent event(key, mods, scanCode);
            send_window_event(event, glfwWindow);
        }
    }

    void on_window_close(GLFWwindow* glfwWindow) {
        WindowCloseEvent event{};
        send_window_event(event, glfwWindow);
    }

    void send_window_event(Event& event, GLFWwindow* glfwWindow) {
        DD_LOG_TRACE(event.toString());
        auto w = (window*) glfwGetWindowUserPointer(glfwWindow);
        w->config.on_event(event);
    }
}
