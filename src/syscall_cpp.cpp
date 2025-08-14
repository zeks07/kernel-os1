#include "../h/syscall_cpp.hpp"

#include "../lib/console.h"

auto operator new(const size_t size) -> void* {
  return mem_alloc(size);
}

auto operator delete(void* ptr) -> void {
  mem_free(ptr);
}

Thread::Thread(void (*body)(void*), void* arg)
  : my_handle(nullptr), my_body(body), my_arg(arg) {
}

Thread::~Thread() {
  delete my_handle;
}

int Thread::start() {
  return thread_create(&my_handle, my_body, my_arg);
}

void Thread::dispatch() {
  thread_dispatch();
}

int Thread::sleep(const time_t timeout) {
  return time_sleep(timeout);
}

Thread::Thread()
  : my_handle(nullptr),
    my_body([](void* arg) {
      static_cast<Thread*>(arg)->run();
    }),
    my_arg(this) {
}

Semaphore::Semaphore(const unsigned init) : my_handle(nullptr) {
  sem_open(&my_handle, init);
}

Semaphore::~Semaphore() {
  sem_close(my_handle);
}

int Semaphore::wait() const {
  return sem_wait(my_handle);
}

int Semaphore::signal() const {
  return sem_signal(my_handle);
}

int Semaphore::timedWait(const time_t timeout) const {
  return sem_timedwait(my_handle, timeout);
}

int Semaphore::tryWait() const {
  return sem_trywait(my_handle);
}

char Console::getc() {
  return __getc();
}

void Console::putc(const char character) {
  __putc(character);
}
