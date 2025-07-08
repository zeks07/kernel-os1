#include "../../h/console/Console.hpp"

#include "../../h/Kernel.hpp"
#include "../../lib/console.h"


void Console::handle() {
  kernel::Kernel::disable_interrupts();
  console_handler();
  kernel::Kernel::enable_interrupts();
}
