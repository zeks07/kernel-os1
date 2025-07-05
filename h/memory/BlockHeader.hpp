#pragma once

#include "../../lib/hw.h"


namespace kernel {
  class BlockHeader;

  auto new_block_header_at(void* ptr, size_t size, bool used = false) -> BlockHeader*;

  class BlockHeader {
  public:
    size_t size;
    BlockHeader* next;
    BlockHeader* previous;
    bool is_used;

    explicit BlockHeader(const size_t size, const bool used = false)
      : size(size), next(nullptr), previous(nullptr), is_used(used) {}

    auto link_to(BlockHeader* next) -> void;
    auto free() -> void;
  };
}
