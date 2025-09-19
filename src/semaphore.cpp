#include "../h/semaphore.hpp"

#include "../h/syscall_c.h"
#include "../h/thread.hpp"

namespace kernel {
  namespace sync {
    Semaphore::Semaphore(const unsigned init)
      : value(init), state(State::Open) {
    }

    auto Semaphore::wait() -> int {
      if (state == State::Closed) return -1;

      value--;

      if (value >= 0) return 0;

      const auto running = thread::get_running();
      running->block();
      wait_queue.add(running);

      thread_dispatch();

      return 0;
    }

    auto Semaphore::signal() -> int {
      if (state == State::Closed) return -1;

      value++;

      if (value > 0) return 0;

      wake_first();
      return 0;
    }

    auto Semaphore::close() -> int {
      if (state == State::Closed) return -1;

      state = State::Closed;
      while (!wait_queue.is_empty()) wake_first();
      return 0;
    }

    auto Semaphore::wake_first() -> void {
      const auto unblocked = wait_queue.remove_first();
      if (unblocked) unblocked->wake(thread::WakeReason::Signal);
    }

    auto open_semaphore(Semaphore** handle, const unsigned init) -> int {
      *handle = new Semaphore(init);
      return *handle ? 0 : -1;
    }
  }
}
