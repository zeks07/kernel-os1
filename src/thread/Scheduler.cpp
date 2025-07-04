#include "../../h//thread/Scheduler.hpp"

#include "../../h/thread/TCB.hpp"


auto thread::Scheduler::put_thread(TCB* thread) -> void {
  ready_queue->add(thread);
}

auto thread::Scheduler::get_thread() -> TCB* {
  if (ready_queue->is_empty()) {
    return nullptr;
  }
  return ready_queue->remove_first();
}

auto thread::Scheduler::dispatch() -> void {
  TCB* old_thread = running_thread;
  if (!old_thread->is_finished()) {
    put_thread(old_thread);
  }
  running_thread = get_thread();
  switch_context(old_thread, running_thread);
}

auto thread::Scheduler::get_running_thread() -> TCB* {
  return running_thread;
}

struc::List<thread::TCB*>* thread::Scheduler::ready_queue;
thread::TCB* thread::Scheduler::running_thread = nullptr;
