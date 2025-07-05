#pragma once
#include "../structure/List.hpp"

class Kernel;

namespace kernel::thread {
  class TCB;

  class Scheduler {
  public:
    static auto init(TCB* kernel_thread) -> void;

    static auto put_thread(TCB* thread) -> void;
    static auto get_thread() -> TCB*;
    static auto dispatch() -> void;

    static auto get_running_thread() -> TCB*;

    static auto destructor() -> void;

  private:
    static struc::List<TCB*>* ready_queue;
    static TCB* running_thread;
  };
}
