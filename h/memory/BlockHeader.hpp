#pragma once

#include "../../lib/hw.h"


constexpr size_t BLOCK_HEADER_SIZE = 32;

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

auto new_block_header_at(void* ptr, size_t size, bool used = false) -> BlockHeader*;
