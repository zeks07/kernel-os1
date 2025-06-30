#include "../h/memory/MemoryAllocator.hpp"


#define DIRECT_MODE 0b00


extern "C" void trap_vector_base();

auto init() -> void {
  auto trap_vector_base_pointer = reinterpret_cast<uint64>(&trap_vector_base) | DIRECT_MODE;
  __asm__ volatile("csrw stvec, %0" : : "r"(trap_vector_base_pointer));
  MemoryAllocator::init();
}

auto main() -> void {
  init();
}
