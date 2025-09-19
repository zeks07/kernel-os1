#pragma once

#include "collections/List.hpp"

namespace kernel {
  namespace thread {
    class TCB;
  }

  namespace sync {
    class Semaphore {
    public:
      explicit Semaphore(unsigned init = 1);

      auto wait() -> int;
      auto signal() -> int;

      auto close() -> int;
    private:
      enum class State {
        Open, Closed
      };

      long value;
      collection::List<thread::TCB*> wait_queue;
      State state;

      auto wake_first() -> void;
    };

    auto open_semaphore(Semaphore** handle, unsigned init) -> int;
  }
}

using _sem = kernel::sync::Semaphore;