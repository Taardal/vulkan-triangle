#include "clock.h"

namespace dd {

    void start_clock(clock& clock) {
      clock.start_time = std::chrono::high_resolution_clock::now();
    }
}
