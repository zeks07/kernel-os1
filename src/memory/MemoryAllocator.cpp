#include "../../h/memory/MemoryAllocator.hpp"

#include "../../lib/mem.h"

#include "../../h/print/print.hpp"
#include "../../h/memory/BlockHeader.hpp"


namespace {
  constexpr size_t BLOCK_HEADER_SIZE = 32;
}

auto kernel::MemoryAllocator::init() -> void {
  heap_start = const_cast<void*>(HEAP_START_ADDR);
  heap_end = const_cast<void*>(HEAP_END_ADDR);

  heap_size = reinterpret_cast<size_t>(heap_end) - reinterpret_cast<size_t>(heap_start);

  block_header_list_head = new_block_header_at(heap_start, heap_size);
}

auto kernel::MemoryAllocator::mem_alloc(const size_t size) -> void* {
  const auto number_of_blocks = (size + BLOCK_HEADER_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
  if (!block_header_list_head || number_of_blocks <= 0) {
    return nullptr;
  }

  const auto actual_size = number_of_blocks * MEM_BLOCK_SIZE;

  auto current = block_header_list_head;

  while (current) {
    if (!current->is_used && current->size >= actual_size) break;
    current = current->next;
  }

  if (!current) {
    return nullptr;
  }

  if (current->size > actual_size) {
    auto* new_block_header = new_block_header_at(reinterpret_cast<char*>(current) + actual_size, current->size - actual_size);
    current->link_to(new_block_header);
  }

  current->size = actual_size;
  current->is_used = true;

  return reinterpret_cast<char*>(current) + BLOCK_HEADER_SIZE;
}

auto kernel::MemoryAllocator::mem_free(void* ptr) -> int {
  if (!ptr) {
    return -1;
  }

  auto* block = reinterpret_cast<BlockHeader*>(static_cast<char*>(ptr) - BLOCK_HEADER_SIZE);
  block->free();

  return 0;
}

auto kernel::MemoryAllocator::print_memory_usage() -> void {
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

void* kernel::MemoryAllocator::heap_start;
void* kernel::MemoryAllocator::heap_end;
kernel::BlockHeader* kernel::MemoryAllocator::block_header_list_head;
uint64 kernel::MemoryAllocator::heap_size;
