#include "../../h//thread/Scheduler.hpp"

#include "../../h/Kernel.hpp"
#include "../../h/thread/TCB.hpp"

auto kernel::thread::Scheduler::init(TCB* kernel_thread) -> void {
  ready_queue = new util::List<TCB*>();
  running_thread = kernel_thread;
}

auto kernel::thread::Scheduler::put_thread(TCB* thread) -> void {
  ready_queue->add(thread);
}

auto kernel::thread::Scheduler::get_thread() -> TCB* {
  if (ready_queue->is_empty()) {
    return nullptr;
  }
  return ready_queue->remove_first();
}

auto kernel::thread::Scheduler::dispatch() -> void {
  const auto old_thread = running_thread;
  if (old_thread->is_ready()) {
    put_thread(old_thread);
  }
  running_thread = get_thread();
  switch_context(old_thread, running_thread);
  time_since_last_dispatch = 0;
}

auto kernel::thread::Scheduler::get_running_thread() -> TCB* {
  return running_thread;
}

auto kernel::thread::Scheduler::destructor() -> void {
  delete ready_queue;
}

auto kernel::thread::Scheduler::tick() -> void {
  time_since_last_dispatch++;
  if (time_since_last_dispatch >= 10) {
    uint64 sepc;
    __asm__ volatile ("csrr %0, sepc" : "=r"(sepc));
    dispatch();
    __asm__ volatile ("csrw sepc, %0" :: "r"(sepc));
  }
}

util::List<kernel::thread::TCB*>* kernel::thread::Scheduler::ready_queue;
kernel::thread::TCB* kernel::thread::Scheduler::running_thread = nullptr;
time_t kernel::thread::Scheduler::time_since_last_dispatch = 0;
