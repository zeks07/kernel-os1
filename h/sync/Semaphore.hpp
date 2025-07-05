#pragma once
#include "../structure/List.hpp"


namespace kernel::thread {
  class TCB;
}

namespace kernel::sync {
  class Semaphore;

  using time_t = unsigned long;

  auto open_semaphore(Semaphore** handle, unsigned init) -> int;

  class Semaphore {
  public:
    explicit Semaphore(unsigned init);

    auto wait() -> void;
    auto signal() -> void;

  private:
    enum State {
      Open, Closed
    };

    auto close() -> int;

    auto is_open() const -> bool;
    auto is_closed() const -> bool;

    auto wake_first() -> void;

    long value;
    struc::List<thread::TCB*> wait_queue;
    State state;
  };
}

using _sem = kernel::sync::Semaphore;
