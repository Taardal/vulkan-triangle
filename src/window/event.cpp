#include "event.h"

namespace dd {
    Event::Event(EventType type) : type(type), name(getName(type)) {
    }

    std::string Event::toString() const {
        std::stringstream ss;
        ss << name << "{}";
        return ss.str();
    }

    std::string Event::getName(EventType eventType) {
        switch (eventType) {
            case EventType::KeyPressed:
                return "KeyPressedEvent";
            case EventType::KeyReleased:
                return "KeyReleasedEvent";
            case EventType::KeyRepeated:
                return "KeyRepeatedEvent";
            case EventType::KeyTyped:
                return "KeyTypedEvent";
            case EventType::MouseMoved:
                return "MouseMovedEvent";
            case EventType::MouseScrolled:
                return "MouseScrolledEvent";
            case EventType::MouseButtonPressed:
                return "MouseButtonPressedEvent";
            case EventType::MouseButtonReleased:
                return "MouseButtonReleasedEvent";
            case EventType::WindowClose:
                return "WindowCloseEvent";
            case EventType::WindowMinimize:
                return "WindowMinimizeEvent";
            case EventType::WindowResize:
                return "WindowResizeEvent";
            default:
                return "";
        }
    }
}

namespace dd {
    std::ostream& operator<<(std::ostream& os, const Event& event) {
        os << event.toString();
        return os;
    }
}
