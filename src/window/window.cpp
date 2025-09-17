#include "window.h"
#include "key_event.h"
#include "window_event.h"

#include <GLFW/glfw3.h>

namespace dd {

    void initialize_glfw(window& window) {
        DD_ASSERT(glfwInit(), "Could not initialize GLFW");

        glfwSetErrorCallback(on_glfw_error);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, window.config.resizable);
        glfwWindowHint(GLFW_MAXIMIZED, window.config.maximized);

        GLFWmonitor* fullscreen_monitor = nullptr;
        GLFWwindow* shared_window = nullptr;
        GLFWwindow* glfw_window = glfwCreateWindow(
            window.config.width,
            window.config.height,
            window.config.title.c_str(),
            fullscreen_monitor,
            shared_window
        );
        DD_ASSERT(glfw_window != nullptr, "Could not create GLFW window");

        glfwSetWindowUserPointer(glfw_window, &window);
        glfwSetKeyCallback(glfw_window, on_glfw_key_change_event);
        glfwSetWindowCloseCallback(glfw_window, on_glfw_window_close_event);

        window.glfw_window = glfw_window;
    }

    void terminate_glfw(window& window) {
        glfwDestroyWindow(window.glfw_window);
        glfwTerminate();
    }

    void poll_events() {
        glfwPollEvents();
    }

    void on_glfw_error(int32_t error, const char* description) {
       std::cerr << "GLFW error: [" << error << "]" << description << std::endl;
    }

    void on_glfw_key_change_event(GLFWwindow* glfw_window, int32_t key, int32_t scan_code, int32_t action, int32_t mods) {
        if (action == GLFW_PRESS) {
            KeyPressedEvent event(key, mods, scan_code);
            on_glfw_event(event, glfw_window);
        } else if (action == GLFW_RELEASE) {
            KeyReleasedEvent event(key, mods, scan_code);
            on_glfw_event(event, glfw_window);
        } else if (action == GLFW_REPEAT) {
            KeyRepeatedEvent event(key, mods, scan_code);
            on_glfw_event(event, glfw_window);
        }
    }

    void on_glfw_window_close_event(GLFWwindow* glfw_window) {
        WindowCloseEvent event{};
        on_glfw_event(event, glfw_window);
    }

    void on_glfw_event(Event& event, GLFWwindow* glfw_window) {
        auto w = (window*) glfwGetWindowUserPointer(glfw_window);
        w->config.on_event(event);
    }
}
