#include "../h/syscall_c.hpp"

#include "../h/memory/BlockHeader.hpp"

uint64 syscall(uint64 operation_code, uint64 arg0 = 0, uint64 arg1 = 0, uint64 arg2 = 0, uint64 arg3 = 0) {
  uint64 ret;
  __asm__ volatile ("ecall");
  __asm__ volatile ("mv %0, a0" : "=r"(ret));
  return ret;
}

void* mem_alloc(const size_t size) {
  const uint64 number_of_blocks = (size + BLOCK_HEADER_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
  return reinterpret_cast<void*>(syscall(0x01, number_of_blocks));
}

int mem_free(void* ptr) {
  return static_cast<int>(syscall(0x02, reinterpret_cast<uint64>(ptr)));
}
