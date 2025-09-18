#include "window.h"
#include "key_event.h"
#include "window_event.h"

#include <GLFW/glfw3.h>

namespace Game {
    void on_glfw_error(i32 error, const char* description) {
        std::cerr << "GLFW error: [" << error << "]" << description << std::endl;
    }

    void on_glfw_event(Event& event, GLFWwindow* glfw_window) {
        auto window = (Window*) glfwGetWindowUserPointer(glfw_window);
        window->on_event(event);
    }

    void on_glfw_key_change_event(GLFWwindow* glfw_window, i32 key, i32 scan_code, i32 action, i32 mods) {
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

    Window create_window(const WindowConfig& config) {
        Window window{};
        window.config = config;

        if (!glfwInit()) {
            GM_THROW("Could not initialize GLFW");
        }

        glfwSetErrorCallback(on_glfw_error);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, config.resizable);
        glfwWindowHint(GLFW_MAXIMIZED, config.maximized);

        GLFWmonitor* fullscreen_monitor = nullptr;
        GLFWwindow* shared_window = nullptr;
        window.glfw_window = glfwCreateWindow(
            config.width,
            config.height,
            config.title.c_str(),
            fullscreen_monitor,
            shared_window
        );
        if (!window.glfw_window) {
            GM_THROW("Could not create GLFW window");
        }

        glfwSetKeyCallback(window.glfw_window, on_glfw_key_change_event);
        glfwSetWindowCloseCallback(window.glfw_window, on_glfw_window_close_event);

        return window;
    }

    void destroy_window(const Window& window) {
        glfwDestroyWindow(window.glfw_window);
        glfwTerminate();
    }

    void set_window_event_listener(Window& window, const std::function<void(Event&)>& on_event) {
        window.on_event = on_event;
        glfwSetWindowUserPointer(window, &window);
    }
}
