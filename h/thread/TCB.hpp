#pragma once

#include "../../lib/hw.h"

namespace kernel::thread {
  class TCB;

  using body = void (*)(void*);

  auto create_thread(TCB** handle, body body, void* arg) -> int;
  auto exit_thread() -> void;

  auto thread_wrapper() -> void;

  auto switch_context(TCB* new_thread, TCB* old_thread) -> void;

  enum class WakeReason {
    None, Signal, Timeout
  };

  class TCB {
    enum State { Ready, Waiting, Finished };

  public:
    struct Context {
      uint64 ra;
      uint64 sp;
    };

    TCB(int id, body body, void* arg, size_t stack_size);

    auto get_id() const -> int;
    auto get_state() const -> State;

    auto is_ready() const -> bool;
    auto finish() -> void;
    auto is_finished() const -> bool;
    auto block() -> void;
    auto is_blocked() const -> bool;

    auto wake(WakeReason reason) -> void;
    auto get_wake_reason() const -> WakeReason;

    auto run() const -> void;

    ~TCB();

  private:
    int id;

    State state;

    body thread_body;
    void* thread_arguments;

    Context context{};
    size_t stack_size;
    void* stack;

    WakeReason wake_reason = WakeReason::None;
  };
}

using _thread = kernel::thread::TCB;
