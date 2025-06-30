#include "../h/memory/MemoryAllocator.hpp"
#include "../h/print/print.hpp"


auto init() -> void {
  MemoryAllocator::init();
}

auto main() -> void {
  init();
}
