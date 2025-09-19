#include "../h/syscall_c.h"

namespace {
  auto syscall(
    uint64 operation_code,
    uint64 argument_0 = 0,
    uint64 argument_1 = 0,
    uint64 argument_2 = 0
  ) -> uint64 {
    uint64 result;
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %0, a0" : "=r"(result));
    return result;
  }
}

auto mem_alloc(const size_t size) -> void* {
  return reinterpret_cast<void*>(
    syscall(0x01, size)
  );
}

auto mem_free(void* pointer) -> int {
  return static_cast<int>(
    syscall(
      0x02,
      reinterpret_cast<uint64>(pointer)
    )
  );
}

auto mem_get_free_space() -> size_t {
  return static_cast<size_t>(
    syscall(0x03)
  );
}

auto mem_get_largest_free_block() -> size_t {
  return static_cast<size_t>(
    syscall(0x04)
  );
}

auto thread_create(thread_t* handle, void(* start_routine)(void*), void* arg) -> int {
  return static_cast<int>(
    syscall(
      0x11,
      reinterpret_cast<uint64>(handle),
      reinterpret_cast<uint64>(start_routine),
      reinterpret_cast<uint64>(arg)
    )
  );
}

auto thread_exit() -> int {
  return static_cast<int>(
    syscall(0x12)
  );
}

auto thread_dispatch() -> void {
  syscall(0x13);
}

auto sem_open(sem_t* handle, const unsigned init) -> int {
  return static_cast<int>(
    syscall(
      0x21,
      reinterpret_cast<uint64>(handle),
      init
    )
  );
}

auto sem_close(sem_t handle) -> int {
  return static_cast<int>(
    syscall(
      0x22,
      reinterpret_cast<uint64>(handle)
    )
  );
}

auto sem_wait(sem_t id) -> int {
  return static_cast<int>(
    syscall(
      0x23,
      reinterpret_cast<uint64>(id)
    )
  );
}

auto sem_signal(sem_t id) -> int {
  return static_cast<int>(
    syscall(
      0x24,
      reinterpret_cast<uint64>(id)
    )
  );
}

auto time_sleep(const time_t timeout) -> int {
  return static_cast<int>(
    syscall(
      0x31,
      timeout
    )
  );
}

auto getc() -> char {
  return static_cast<char>(
    syscall(0x41)
  );
}

auto putc(const char chr) -> void {
  syscall(
    0x42,
    static_cast<uint64>(chr)
  );
}
