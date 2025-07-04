#include "../h/Kernel.hpp"

#include "../h/syscall_cpp.hpp"
#include "../h/print/print.hpp"
#include "../h/thread/Scheduler.hpp"


static volatile bool finished1 = false;
static volatile bool finished2 = false;

auto thread_function1(void* arg) -> void {
  for (uint64 i = 0; i < 10; i++) {
    println("A: i=%d", i);
    for (uint64 j = 0; j < 10000; j++) {
      for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
      thread_dispatch();
    }
  }
  println("A finished!");
  finished1 = true;
}

class WorkerB final : public Thread {
  static auto thread_function2(void* arg) -> void;
public:
  WorkerB() = default;

  auto run() -> void override {
    thread_function2(nullptr);
  }
};

auto WorkerB::thread_function2(void*) -> void {
  for (uint64 i = 0; i < 16; i++) {
    println("B: i=%d", i);
    for (uint64 j = 0; j < 10000; j++) {
      for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
      thread_dispatch();
    }
  }
  println("B finished!");
  finished2 = true;
  thread_dispatch();
}

auto Kernel::run() -> void {
  println("Main start.");

  thread::Scheduler::ready_queue = new struc::List<thread::TCB*>();

  thread_t kernel_thread;

  thread::create_thread(&kernel_thread, nullptr, nullptr);
  thread::Scheduler::running_thread = kernel_thread;

  Thread thread1(&thread_function1, nullptr);
  WorkerB thread2;

  thread1.start();
  thread2.start();

  while (finished1 == false || finished2 == false) {
    Thread::dispatch();
  }

  println("Main done.");

  delete thread::Scheduler::ready_queue;
}
