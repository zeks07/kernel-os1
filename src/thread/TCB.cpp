#include "../../h/thread/TCB.hpp"

#include "../../h/syscall_c.hpp"
#include "../../h/thread/Scheduler.hpp"

auto pop_spp_spie() -> void {
  __asm__ volatile ("csrw sepc, ra");
  __asm__ volatile ("sret");
}

// ReSharper disable once CppDFAConstantFunctionResult
auto thread::create_thread(TCB** handle, const body body, void* arg) -> int {
  static int id = 0;

  const auto new_thread = new TCB(id++, body, arg, DEFAULT_STACK_SIZE);
  // ReSharper disable once CppDFAConstantConditions
  if (!new_thread) {
    // ReSharper disable once CppDFAUnreachableCode
    return -1;
  }

  if (body != nullptr) {
    Scheduler::put_thread(new_thread);
  }

  *handle = new_thread;
  return 0;
}

auto thread::exit_thread() -> void {
  TCB* running_thread = Scheduler::get_running_thread();
  running_thread->finish();
  Scheduler::dispatch();
}

auto thread::thread_wrapper() -> void {
  pop_spp_spie();

  const TCB* running_thread = Scheduler::get_running_thread();
  running_thread->run();

  exit_thread();
}

thread::TCB::TCB(const int id, const body body, void* arg, const size_t stack_size)
  : id(id), state(Ready), thread_body(body), thread_arguments(arg), stack_size(stack_size) {

  if (body == nullptr) {
    stack = nullptr;
    context = {0, 0};
    return;
  }

  stack = mem_alloc(stack_size);
  context = {
    reinterpret_cast<uint64>(&thread_wrapper),
    reinterpret_cast<uint64>(stack) + stack_size - 1
  };
}

auto thread::TCB::get_state() const -> State {
  return state;
}

auto thread::TCB::finish() -> void {
  state = Finished;
}

auto thread::TCB::is_finished() const -> bool {
  return state == Finished;
}

auto thread::TCB::run() const -> void {
  thread_body(thread_arguments);
}

thread::TCB::
~TCB() {
  mem_free(stack);
}
