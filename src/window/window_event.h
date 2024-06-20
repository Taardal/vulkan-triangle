#pragma once

#include "event.h"

namespace dd {
    struct WindowCloseEvent final : Event {
        WindowCloseEvent();
    };
}

namespace dd {
    struct WindowMinimizeEvent final : Event {
        bool minimized;

        explicit WindowMinimizeEvent(bool minimized);

        std::string toString() const override;
    };

    std::ostream& operator<<(std::ostream& os, const WindowMinimizeEvent& event);
}

namespace dd {
    struct WindowResizeEvent final : Event {
        int32_t width;
        int32_t height;

        WindowResizeEvent(int32_t width, int32_t height);

        std::string toString() const override;
    };

    std::ostream& operator<<(std::ostream& os, const WindowResizeEvent& event);
}
