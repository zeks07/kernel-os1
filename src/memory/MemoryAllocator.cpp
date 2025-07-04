#include "../../h/memory/MemoryAllocator.hpp"

#include "../../lib/mem.h"

#include "../../h/print/print.hpp"
#include "../../h/memory/BlockHeader.hpp"


auto MemoryAllocator::init() -> void {
  heap_start = const_cast<void*>(HEAP_START_ADDR);
  heap_end = const_cast<void*>(HEAP_END_ADDR);

  heap_size = reinterpret_cast<size_t>(heap_end) - reinterpret_cast<size_t>(heap_start);

  block_header_list_head = new_block_header_at(heap_start, heap_size);
}

auto MemoryAllocator::mem_alloc(const size_t number_of_blocks) -> void* {
  return __mem_alloc(number_of_blocks);
  // if (!block_header_list_head || number_of_blocks <= 0) {
  //   return nullptr;
  // }
  //
  // const auto size = number_of_blocks * MEM_BLOCK_SIZE;
  //
  // auto current = block_header_list_head;
  //
  // while (current) {
  //   if (!current->is_used && current->size >= size) break;
  //   current = current->next;
  // }
  //
  // if (!current) {
  //   return nullptr;
  // }
  //
  // if (current->size > size) {
  //   auto* new_block_header = new_block_header_at(reinterpret_cast<char*>(current) + size, current->size - size);
  //   current->link_to(new_block_header);
  // }
  //
  // current->size = size;
  // current->is_used = true;
  //
  // return reinterpret_cast<char*>(current) + BLOCK_HEADER_SIZE;
}

auto MemoryAllocator::mem_free(void* ptr) -> int {
  return __mem_free(ptr);
  // if (!ptr) {
  //   return -1;
  // }
  //
  // auto* block = reinterpret_cast<BlockHeader*>(static_cast<char*>(ptr) - BLOCK_HEADER_SIZE);
  // block->free();
  //
  // return 0;
}

auto MemoryAllocator::print_memory_usage() -> void {
  println("Total memory size: %d (%x) B", heap_size, heap_size);
  println("---------");

  BlockHeader* current = block_header_list_head;
  for (int i = 0; current; ++i) {
    const auto address = reinterpret_cast<long>(current) - reinterpret_cast<long>(heap_start);
    const auto previous = current->previous ? reinterpret_cast<long>(current->previous) - reinterpret_cast<long>(heap_start) : -1;
    const auto next = current->next ? reinterpret_cast<long>(current->next) - reinterpret_cast<long>(heap_start) : -1;
    println("Memory Block Header %d: (%s)", i, current->is_used ? "used" : "free");
    println("At: %x", address);
    println("Size: %d (%x) B", current->size, current->size);
    println("Previous: %x", previous);
    println("Next: %x", next);
    println("---------");
    current = current->next;
  }
}

void* MemoryAllocator::heap_start;
void* MemoryAllocator::heap_end;
BlockHeader* MemoryAllocator::block_header_list_head;
uint64 MemoryAllocator::heap_size;
