#include "../h/syscall_cpp.hpp"

#include "../h/syscall_c.h"

auto operator new(const size_t size) -> void* {
  return mem_alloc(size);
}

auto operator new [](const size_t size) -> void* {
  return mem_alloc(size);
}

auto operator delete(void* pointer) noexcept-> void {
  mem_free(pointer);
}

auto operator delete [](void* pointer) noexcept -> void {
  mem_free(pointer);
}

Thread::Thread(void(* body)(void*), void* arg)
  : my_handle(nullptr), my_body(body), my_arg(arg) {
}

Thread::~Thread() {
  delete my_handle;
}

auto Thread::start() -> int {
  return thread_create(&my_handle, my_body, my_arg);
}

auto Thread::dispatch() -> void {
  thread_dispatch();
}

auto Thread::sleep(time_t) -> int {
  return -1;
}

Thread::Thread()
  : my_handle(nullptr),
    my_body([](void* arg) {
      static_cast<Thread*>(arg)->run();
    }),
    my_arg(this) {
}

auto Thread::run() -> void {
}

auto PeriodicThread::terminate() -> void {
  period = 0;
}

PeriodicThread::PeriodicThread(const time_t period) : period(period) {
}

auto PeriodicThread::periodicActivation() -> void {
}

auto PeriodicThread::run() -> void {
  while (period > 0) {
    time_sleep(period);
    periodicActivation();
  }
}

Semaphore::Semaphore(const unsigned init) : my_handle(nullptr) {
  sem_open(&my_handle, init);
}

Semaphore::~Semaphore() {
  sem_close(my_handle);
}

auto Semaphore::wait() const -> int {
  return sem_wait(my_handle);
}

auto Semaphore::signal() const -> int {
  return sem_signal(my_handle);
}

auto Console::getc() -> char {
  return ::getc();
}

auto Console::putc(const char chr) -> void {
  ::putc(chr);
}