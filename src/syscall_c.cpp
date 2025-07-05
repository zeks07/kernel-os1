#include "../h/syscall_c.hpp"

#include "../h/memory/BlockHeader.hpp"

auto syscall(uint64 operation_code, uint64 arg0 = 0, uint64 arg1 = 0, uint64 arg2 = 0, uint64 arg3 = 0) -> uint64 {
  uint64 ret;
  __asm__ volatile ("ecall");
  __asm__ volatile ("mv %0, a0" : "=r"(ret));
  return ret;
}

auto mem_alloc(const size_t size) -> void* {
  return reinterpret_cast<void*>(syscall(0x01, size));
}

auto mem_free(void* ptr) -> int {
  return static_cast<int>(syscall(0x02, reinterpret_cast<uint64>(ptr)));
}

auto thread_create(thread_t* handle, void (*start_routine)(void*), void* arg) -> int {
  return static_cast<int>(syscall(
    0x11,
    reinterpret_cast<uint64>(handle),
    reinterpret_cast<uint64>(start_routine),
    reinterpret_cast<uint64>(arg)
  ));
}

auto thread_exit() -> int {
  return static_cast<int>(syscall(0x12));
}

auto thread_dispatch() -> void {
  syscall(0x13);
}

auto sem_open(sem_t* handle, const unsigned init) -> int {
  return static_cast<int>(syscall(
    0x21,
    reinterpret_cast<uint64>(handle),
    init
  ));
}

auto sem_close(sem_t handle) -> int {
  return static_cast<int>(syscall(
    0x22,
    reinterpret_cast<uint64>(handle)
  ));
}

auto sem_wait(sem_t id) -> int {
  return static_cast<int>(syscall(
    0x23,
    reinterpret_cast<uint64>(id)
  ));
}

auto sem_signal(sem_t id) -> int {
  return static_cast<int>(syscall(
    0x24,
    reinterpret_cast<uint64>(id)
  ));
}

auto sem_timedwait(sem_t id, time_t timeout) -> int {
  return 0;
}

auto sem_trywait(sem_t id) -> int {
  return 0;
}

int time_sleep(time_t timeout) {
  return 0;
}
