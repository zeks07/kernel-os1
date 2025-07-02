#include "../h/syscall_c.hpp"

#include "../h/memory/BlockHeader.hpp"

auto syscall(uint64 operation_code, uint64 arg0 = 0, uint64 arg1 = 0, uint64 arg2 = 0, uint64 arg3 = 0) -> uint64 {
  uint64 ret;
  __asm__ volatile ("ecall");
  __asm__ volatile ("mv %0, a0" : "=r"(ret));
  return ret;
}

auto mem_alloc(const size_t size) -> void* {
  const uint64 number_of_blocks = (size + BLOCK_HEADER_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
  return reinterpret_cast<void*>(syscall(0x01, number_of_blocks));
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
  syscall(0x12);
}
