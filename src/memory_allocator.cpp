#include "../h/memory_allocator.hpp"

namespace kernel {
  namespace memory {
    namespace {
      class BlockHeader final {
      public:
        size_t size;
        BlockHeader* next;
        BlockHeader* previous;
        bool is_used;

        auto link_to(BlockHeader* next_block) -> void;
        auto free() -> void;
      };

      auto BlockHeader::link_to(BlockHeader* next_block) -> void {
        next_block->previous = this;
        next_block->next = this->next;
        if (this->next) {
          this->next->previous = next_block;
        }
        this->next = next_block;
      }

      auto BlockHeader::free() -> void {
        is_used = false;

        if (next && !next->is_used) {
          size += next->size;
          next = next->next;
          if (next)
            next->previous = this;
        }

        if (previous && !previous->is_used) {
          previous->size += size;
          previous->next = next;
          if (next)
            next->previous = previous;
        }
      }

      constexpr size_t block_header_size = 32;

      void* heap_start;
      void* heap_end;
      BlockHeader* block_header_list_head;
      uint64 heap_size;
      uint64 available_space;

      auto new_block_header_at(void* pointer, const size_t size) -> BlockHeader* {
        const auto block_header = static_cast<BlockHeader*>(pointer);
        block_header->size = size;
        block_header->is_used = false;
        block_header->previous = nullptr;
        block_header->next = nullptr;
        return block_header;
      }
    }

    auto init() -> void {
      heap_start = const_cast<void*>(HEAP_START_ADDR);
      heap_end = const_cast<void*>(HEAP_END_ADDR);
      available_space = heap_size = reinterpret_cast<size_t>(heap_end) - reinterpret_cast<size_t>(heap_start);

      block_header_list_head = new_block_header_at(heap_start, heap_size);
    }

    auto allocate(const size_t size) -> void* {
      const auto number_of_blocks = (size + block_header_size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

      if (!block_header_list_head || number_of_blocks <= 0) return nullptr;

      const auto actual_size = number_of_blocks * MEM_BLOCK_SIZE;

      auto current = block_header_list_head;
      while (current) {
        if (!current->is_used && current->size >= actual_size) break;
        current = current->next;
      }

      if (!current) return nullptr;

      if (current->size > actual_size) {
        auto* new_block_header = new_block_header_at(reinterpret_cast<char*>(current) + actual_size, current->size - actual_size);
        current->link_to(new_block_header);
      }

      current->size = actual_size;
      available_space -= actual_size;
      current->is_used = true;

      return reinterpret_cast<char*>(current) + block_header_size;
    }

    auto free(void* pointer) -> int {
      if (!pointer) return -1;

      auto* block = reinterpret_cast<BlockHeader*>(static_cast<char*>(pointer) - block_header_size);
      available_space += block->size;
      block->free();

      return 0;
    }

    auto get_free_space() -> size_t {
      return available_space;
    }

    auto get_largest_free_block() -> size_t {
      auto result = 0UL;
      auto current = block_header_list_head;
      while (current) {
        if (!current->is_used && current->size > result) result = current->size;
        current = current->next;
      }
      return result;
    }
  }
}