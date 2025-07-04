#pragma once

#include "../../lib/hw.h"

namespace thread {
  class TCB;

  using body = void (*)(void*);

  auto create_thread(TCB** handle, body body, void* arg) -> int;
  auto exit_thread() -> void;

  auto thread_wrapper() -> void;

  auto switch_context(TCB* new_thread, TCB* old_thread) -> void;;

  class TCB {
  public:
    enum State { Ready, Running, Waiting, Finished };

    struct Context {
      uint64 ra;
      uint64 sp;
    };

    TCB(int id, body body, void* arg, size_t stack_size);

    // ReSharper disable once CppFunctionIsNotImplemented
    auto save_context() -> void;
    // ReSharper disable once CppFunctionIsNotImplemented
    auto load_context() -> void;

    auto get_id() const -> int;
    auto get_state() const -> State;
    auto finish() -> void;
    auto is_finished() const -> bool;

    auto run() const -> void;

    ~TCB();

  private:
    int id;

    State state;

    body thread_body;
    void* thread_arguments;

    Context context;
    size_t stack_size;
    void* stack;
  };
}

using _thread = thread::TCB;
