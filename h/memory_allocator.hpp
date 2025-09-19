#pragma once

#include "../lib/hw.h"

namespace kernel {
  namespace memory {
    auto init() -> void;
    auto allocate(size_t size) -> void*;
    auto free(void* pointer) -> int;
    auto get_free_space() -> size_t;
    auto get_largest_free_block() -> size_t;
  }
}