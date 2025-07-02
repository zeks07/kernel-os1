#include "../h/Kernel.hpp"

#include "../h/syscall_cpp.hpp"
#include "../h/print/print.hpp"
#include "../h/thread/Scheduler.hpp"


static volatile bool finishedA = false;
static volatile bool finishedB = false;

void thread_function1(void* arg) {
  for (uint64 i = 0; i < 10; i++) {
    println("A: i=%d", i);
    for (uint64 j = 0; j < 10000; j++) {
      for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
      thread_dispatch();
    }
  }
  println("A finished!");
  finishedA = true;
}

class WorkerB final : public Thread {
  void workerBodyB(void* arg);
public:
  WorkerB() {}

  void run() override {
    workerBodyB(nullptr);
  }
};

void WorkerB::workerBodyB(void *) {
  for (uint64 i = 0; i < 16; i++) {
    println("B: i=$d", i);
    for (uint64 j = 0; j < 10000; j++) {
      for (uint64 k = 0; k < 30000; k++) { /* busy wait */ }
      thread_dispatch();
    }
  }
  println("B finished!");
  finishedB = true;
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

  while (!finishedA || !finishedB) {
    Thread::dispatch();
  }

  delete thread::Scheduler::ready_queue;

  println("Main done.");
}
