#include "../../h/sync/Semaphore.hpp"

#include "../../h/thread/Scheduler.hpp"
#include "../../h//syscall_cpp.hpp"
#include "../../h/Kernel.hpp"
#include "../../h/thread/TCB.hpp"


auto kernel::sync::open_semaphore(Semaphore** handle, const unsigned init) -> int {
  *handle = new Semaphore(init);
  if (!*handle) {
    return -1;
  }
  return 0;
}

kernel::sync::Semaphore::Semaphore(const unsigned init)
  : value(init), state(Open) {
}

auto kernel::sync::Semaphore::wait() -> int {
  Kernel::disable_interrupts();

  if (is_closed()) {
    Kernel::enable_interrupts();
    return -1;
  }

  value--;

  if (value >= 0) {
    Kernel::enable_interrupts();
    return 0;
  }

  const auto running = thread::Scheduler::get_running_thread();
  running->block();
  wait_queue.add(running);
  Kernel::enable_interrupts();

  thread_dispatch();

  return 0;
}

auto kernel::sync::Semaphore::try_wait() -> int {
  Kernel::disable_interrupts();
  if (value > 0) {
    Kernel::enable_interrupts();
    return wait();
  }
  Kernel::enable_interrupts();
  return 1;
}

auto kernel::sync::Semaphore::signal() -> int {
  Kernel::disable_interrupts();
  if (is_closed()) {
    Kernel::enable_interrupts();
    return -1;
  }

  value++;

  if (value > 0) {
    Kernel::enable_interrupts();
    return 0;
  }

  wake_first();
  Kernel::enable_interrupts();
  return 0;
}

auto kernel::sync::Semaphore::close() -> int {
  Kernel::disable_interrupts();
  if (is_closed()) {
    Kernel::enable_interrupts();
    return -1;
  }
  state = Closed;

  while (!wait_queue.is_empty()) {
    wake_first();
  }
  Kernel::enable_interrupts();

  return 0;
}

auto kernel::sync::Semaphore::is_open() const -> bool {
  return state == Open;
}

auto kernel::sync::Semaphore::is_closed() const -> bool {
  return state == Closed;
}

auto kernel::sync::Semaphore::wake_first() -> void {
  const auto unblocked = wait_queue.remove_first();
  if (unblocked) {
    unblocked->wake(thread::WakeReason::Signal);
  }
}
