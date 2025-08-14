#include "../../h/console/Console.hpp"

#include "../../h/Kernel.hpp"
#include "../../lib/console.h"


void kernel::console::Console::handle() {
  Kernel::disable_interrupts();
  console_handler();
  Kernel::enable_interrupts();
}

auto kernel::console::Console::getc() -> char {
  return __getc();
}

auto kernel::console::Console::putc(const char chr) -> void {
  __putc(chr);
}
