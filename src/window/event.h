#pragma once

#include <string>

namespace dd {
    enum class EventType {
        None = 0,
        KeyPressed,
        KeyReleased,
        KeyRepeated,
        KeyTyped,
        MouseMoved,
        MouseScrolled,
        MouseButton,
        MouseButtonPressed,
        MouseButtonReleased,
        WindowClose,
        WindowMinimize,
        WindowResize,
    };

    struct Event {
    public:
        EventType type;
        std::string name;

    protected:
        explicit Event(EventType type);

        virtual ~Event() = default;

    public:
        virtual std::string toString() const;

        static std::string getName(EventType eventType);

        template<typename T>
        T& as() const {
            auto downcastedEvent = (T*) this;
            return *downcastedEvent;
        }
    };

    std::ostream& operator<<(std::ostream& os, const Event& event);
}
