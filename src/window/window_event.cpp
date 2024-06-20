#include "window_event.h"

namespace dd {
    WindowCloseEvent::WindowCloseEvent()
        : Event(EventType::WindowClose) {
    }
}

namespace dd {
    WindowMinimizeEvent::WindowMinimizeEvent(
        bool minimized
    ) : Event(EventType::WindowMinimize),
        minimized(minimized) {
    }

    std::string WindowMinimizeEvent::toString() const {
        std::stringstream ss;
        ss << name << "{minimized=" << minimized << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const WindowMinimizeEvent& event) {
        os << event.toString();
        return os;
    }
}

namespace dd {
    WindowResizeEvent::WindowResizeEvent(
        int32_t width,
        int32_t height
    ) : Event(EventType::WindowResize),
        width(width),
        height(height) {
    }

    std::string WindowResizeEvent::toString() const {
        std::stringstream ss;
        ss << name << "{width=" << width << ", height=" << height << "}";
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const WindowResizeEvent& event) {
        os << event.toString();
        return os;
    }
}
