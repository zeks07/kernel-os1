#pragma once

#include "../../lib/hw.h"
#include "../structure/SortedList.hpp"


namespace kernel::thread {
  class TCB;
}

namespace kernel::time {
  using time_t = unsigned long;

  class Timer {
  public:
    static auto init() -> void;

    static auto time_sleep(time_t timeout) -> void;

    static auto tick() -> void;

  private:
    static util::SortedList<thread::TCB*, uint64>* sleep_queue;
    static unsigned long long time;
  };
}
