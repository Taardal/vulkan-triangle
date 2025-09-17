#include "clock.h"

namespace Game {

    void start_clock(clock& clock) {
      clock.start_time = std::chrono::high_resolution_clock::now();
    }
}
