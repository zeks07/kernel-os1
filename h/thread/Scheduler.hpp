#pragma once
#include "../structure/List.hpp"

class Kernel;

namespace thread {
  class TCB;

  class Scheduler {
  public:
    static auto put_thread(TCB* thread) -> void;
    static auto get_thread() -> TCB*;
    static auto dispatch() -> void;

    static auto get_running_thread() -> TCB*;

    friend Kernel;

  private:
    static struc::List<TCB*>* ready_queue;
    static TCB* running_thread;
  };

}
