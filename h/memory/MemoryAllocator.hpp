#pragma once

#include "../../lib/hw.h"

namespace kernel {
  class BlockHeader;

  class MemoryAllocator {
  public:
    static auto init() -> void;
    static auto mem_alloc(size_t size) -> void *;
    static auto mem_free(void *ptr) -> int;
    static auto print_memory_usage() -> void;

  private:
    static void *heap_start;
    static void *heap_end;
    static BlockHeader *block_header_list_head;
    static uint64 heap_size;
  };
} // namespace kernel
