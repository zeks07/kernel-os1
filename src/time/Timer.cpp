#include "../../h/time/Timer.hpp"

#include "../../h/Kernel.hpp"
#include "../../h/syscall_c.hpp"
#include "../../h/thread/Scheduler.hpp"
#include "../../h/thread/TCB.hpp"


auto kernel::time::Timer::init() -> void {
  sleep_queue = new util::SortedList<thread::TCB*, unsigned long>;
}

auto kernel::time::Timer::time_sleep(const time_t timeout) -> void {
  Kernel::disable_interrupts();
  const auto thread = thread::Scheduler::get_running_thread();
  thread->block();
  sleep_queue->add(thread, time + timeout);
  Kernel::enable_interrupts();
  thread_dispatch();
}

auto kernel::time::Timer::tick() -> void {
  println("a");
  Kernel::disable_interrupts();
  time++;
  while (!sleep_queue->is_empty() && sleep_queue->get_key(0) >= time) {
    sleep_queue->remove_first()->wake(thread::WakeReason::Timeout);
  }
  Kernel::enable_interrupts();
}

util::SortedList<kernel::thread::TCB*, uint64>* kernel::time::Timer::sleep_queue = nullptr;
unsigned long long kernel::time::Timer::time = 0;
