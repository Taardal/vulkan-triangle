#include "time.h"

namespace Game {
    TimePoint Time::now() {
        return std::chrono::high_resolution_clock::now();
    }

    TimePoint Time::zero() {
        return TimePoint(Duration::zero());
    }
}