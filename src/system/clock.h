#pragma once

#include <chrono>

namespace dd {

    typedef std::chrono::high_resolution_clock::time_point time;
    typedef std::chrono::high_resolution_clock::duration duration;

    typedef std::chrono::nanoseconds ns;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::seconds sec;

    struct clock {
        time start_time;
    };

    void start_clock(clock& clock);

    template<typename T = duration>
    f64 to_duration(const duration& time) {
        return std::chrono::duration_cast<T>(time).count();
    }

    template<typename T = duration>
    f64 get_time(const clock& clock) {
        time now = std::chrono::high_resolution_clock::now();
        return to_duration<T>(now - clock.start_time);
    }
}
