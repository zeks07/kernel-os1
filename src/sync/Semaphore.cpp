#include "../../h/sync/Semaphore.hpp"

#include "../../h/thread/Scheduler.hpp"
#include "../../h//syscall_cpp.hpp"


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

auto kernel::sync::Semaphore::wait() -> void {
  value--;

  if (value >= 0) {
    return;
  }

  const auto running = thread::Scheduler::get_running_thread();
  running->block();
  wait_queue.add(running);
  thread_dispatch();
}

auto kernel::sync::Semaphore::signal() -> void {
  value++;

  if (value > 0) {
    return;
  }

  wake_first();
}

auto kernel::sync::Semaphore::close() -> int {
  if (is_closed()) {
    return -1;
  }
  state = Closed;

  while (!wait_queue.is_empty()) {
    wake_first();
  }

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
    unblocked->unblock();
    thread::Scheduler::put_thread(unblocked);
  }
}
