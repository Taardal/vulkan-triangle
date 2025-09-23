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

    void window_close_callback(GLFWwindow* glfw_window) {
        WindowCloseEvent event{};
        on_glfw_event(event, glfw_window);
    }

    void key_callback(GLFWwindow* glfw_window, i32 key, i32 scan_code, i32 action, i32 mods) {
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

    void framebuffer_size_callback(GLFWwindow* glfw_window, i32 width, i32 height) {
        WindowResizeEvent event(width, height);
        on_glfw_event(event, glfw_window);
    }

    void create_window(Window& window, const WindowConfig& config) {
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

        glfwSetFramebufferSizeCallback(window.glfw_window, framebuffer_size_callback);
        glfwSetKeyCallback(window.glfw_window, key_callback);
        glfwSetWindowCloseCallback(window.glfw_window, window_close_callback);
    }

    void destroy_window(const Window& window) {
        glfwDestroyWindow(window.glfw_window);
        glfwTerminate();
    }

    void set_window_event_listener(Window& window, const std::function<void(Event&)>& on_event) {
        window.on_event = on_event;
        glfwSetWindowUserPointer(window, &window);
    }

    WindowSize get_window_size(const Window& window) {
        i32 width = 0;
        i32 height = 0;
        glfwGetFramebufferSize(window.glfw_window, &width, &height);
        return { width, height };
    }

    void get_window_size(const Window& window, i32* width, i32* height) {
        glfwGetFramebufferSize(window.glfw_window, width, height);
    }

    void wait_until_not_minimized(const Window& window) {
        bool iconified = is_window_iconified(window);

        i32 width = 0;
        i32 height = 0;
        get_window_size(window, &width, &height);

        while (iconified || width == 0 || height == 0) {
            iconified = is_window_iconified(window);
            get_window_size(window, &width, &height);
            glfwWaitEvents();
        }
    }

    bool is_window_iconified(const Window& window) {
        return glfwGetWindowAttrib(window.glfw_window, GLFW_ICONIFIED) == 1;
    }
}
