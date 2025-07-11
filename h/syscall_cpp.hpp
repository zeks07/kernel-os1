#pragma once

#include "syscall_c.hpp"

auto operator new(size_t size) -> void*;
auto operator delete(void* ptr) -> void;

class Thread {
public:
  Thread(void (*body)(void*), void* arg);
  virtual ~Thread();

  int start();

  static void dispatch();
  // static int sleep(time_t);

protected:
  Thread();
  virtual void run() {}

private:
  thread_t my_handle;
  void (* my_body)(void*);
  void* my_arg;
};

class Semaphore {
public:
  explicit Semaphore(unsigned init = 1);
  virtual ~Semaphore();

  int wait() const;
  int signal() const;
  int timedWait(time_t timeout) const;
  int tryWait() const;

private:
  sem_t my_handle;
};
