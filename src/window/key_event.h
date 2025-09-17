#pragma once

#include "event.h"
#include "keyboard.h"

namespace Game {
    struct KeyEvent : Event {
    public:
        Key key;
        int32_t keyCode;
        int32_t mods;
        int32_t scanCode;

        KeyEvent(EventType type, int32_t keyCode, int32_t mods, int32_t scanCode);

        std::string toString() const override;
    };

    std::ostream& operator<<(std::ostream& os, const KeyEvent& event);
}

namespace Game {
    struct KeyPressedEvent final : KeyEvent {
        explicit KeyPressedEvent(int32_t key, int32_t mods, int32_t scanCode);
    };
}

namespace Game {
    struct KeyReleasedEvent final : KeyEvent {
        explicit KeyReleasedEvent(int32_t key, int32_t mods, int32_t scanCode);
    };
}

namespace Game {
    struct KeyRepeatedEvent final : KeyEvent {
        explicit KeyRepeatedEvent(int32_t key, int32_t mods, int32_t scanCode);
    };
}

namespace Game {
    struct KeyTypedEvent final : KeyEvent {
        explicit KeyTypedEvent(int32_t key, int32_t mods, int32_t scanCode);
    };
}
