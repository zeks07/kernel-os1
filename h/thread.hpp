#pragma once

#include "../lib/hw.h"

namespace kernel {
  namespace thread {
    using body = void (*)(void*);

    enum class WakeReason {
      None, Signal, Timeout
    };

    enum class State {
      Ready, Waiting, Finished
    };

    class TCB final {
    public:
      struct Context {
        uint64 ra;
        uint64 sp;
      };

      TCB(int id, body body, void* argument, size_t stack_size);

      auto get_id() const -> int;
      auto get_state() const -> State;

      auto wake(WakeReason reason) -> void;
      auto block() -> void;
      auto finish() -> void;
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

    auto switch_context(TCB* old_thread, TCB* new_thread) -> void;

    auto init() -> void;

    auto create(TCB** handle, body body, void* arg) -> int;
    // auto sleep(time_t timeout) -> void;
    auto exit() -> void;

    auto put(TCB* thread) -> void;
    auto get() -> TCB*;
    auto dispatch() -> void;

    // auto tick() -> void;

    auto get_running() -> TCB*;
  }
}

using _thread = kernel::thread::TCB;