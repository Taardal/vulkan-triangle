#pragma once

namespace Game {
    typedef std::chrono::duration<i64, std::nano> Nanoseconds;
    typedef std::chrono::duration<i64, std::micro> Microseconds;
    typedef std::chrono::duration<f64, std::milli> Milliseconds;
    typedef std::chrono::duration<f64> Seconds;

    typedef Nanoseconds ns;
    typedef Microseconds us; typedef Microseconds μs;
    typedef Milliseconds ms;
    typedef Seconds sec;

    typedef std::chrono::high_resolution_clock::duration Duration;
    typedef std::chrono::high_resolution_clock::time_point TimePoint;

    class Time {
    public:
        static TimePoint now();

        static TimePoint zero();

        template<typename ToDuration, typename FromDuration>
        static ToDuration as(FromDuration from_duration) {
            return std::chrono::duration_cast<ToDuration>(from_duration);
        }
    };
}