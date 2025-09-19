#pragma once

#include "syscall_c.h"
#include "../lib/hw.h"

[[nodiscard]] auto operator new(size_t size) -> void*;
[[nodiscard]] auto operator new [](size_t size) -> void*;
auto operator delete(void* pointer) noexcept -> void;
auto operator delete [](void* pointer) noexcept -> void;


class Thread {
public:
  Thread(void (* body)(void*), void* arg);
  virtual ~Thread();

  auto start() -> int;

  static auto dispatch() -> void;
  static auto sleep(time_t) -> int;

protected:
  Thread();
  virtual auto run() -> void;

private:
  thread_t my_handle;
  void (* my_body)(void*);
  void* my_arg;
};


class PeriodicThread : public Thread {
public:
  auto terminate() -> void;

protected:
  explicit PeriodicThread(time_t period);
  virtual auto periodicActivation() -> void;

private:
  auto run() -> void final;
  time_t period;
};


class Semaphore {
public:
  explicit Semaphore(unsigned init = 1);
  virtual ~Semaphore();

  auto wait() const -> int;
  auto signal() const -> int;

private:
  sem_t my_handle;
};


class Console {
public:
  static auto getc() -> char;
  static auto putc(char chr) -> void;
};