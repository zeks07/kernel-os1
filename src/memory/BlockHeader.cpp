#include "../../h/memory/BlockHeader.hpp"

auto kernel::new_block_header_at(void* ptr, const size_t size, const bool used) -> BlockHeader* {
  const auto block_header = static_cast<BlockHeader*>(ptr);
  block_header->size = size;
  block_header->is_used = used;
  block_header->previous = nullptr;
  block_header->next = nullptr;
  return block_header;
}

auto kernel::BlockHeader::link_to(BlockHeader* next) -> void {
  next->previous = this;
  next->next = this->next;
  if (this->next) {
    this->next->previous = next;
  }
  this->next = next;
}

auto kernel::BlockHeader::free() -> void {
  is_used = false;

  if (next && !next->is_used) {
    size += next->size;
    next = next->next;
    if (next) next->previous = this;
  }

  if (previous && !previous->is_used) {
    previous->size += size;
    previous->next = next;
    if (next) next->previous = previous;
  }
}
