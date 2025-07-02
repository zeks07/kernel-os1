#include "../h/print/print.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"

void error() {
  println("An error has occurred");

  uint64 status;
  uint64 cause;

  __asm__ volatile ("csrr %[status], sstatus":[status]"=r"(status));
  __asm__ volatile ("csrr %[cause], scause":[cause]"=r"(cause));

  bool in_system_mode = (status & 1 << 8) == 1 << 8;
  bool masked_interrupts = (status & 1 << 1) == 1 << 1;
  bool previous_interrupt_enabled = (status & 1 << 5) == 1 << 5;

  bool is_interrupt = (cause & 1 << 31) == static_cast<uint64>(1) << 31;
  uint64 interrupt_code = cause & 0x7FFFFFFF;

  println("Status: %s", in_system_mode ? "System" : "User");
  println("Interrupts: %s", masked_interrupts ? "Masked" : "Unmasked");
  println("Previous Interrupts: %s", previous_interrupt_enabled ? "Enabled" : "Disabled");

  println("Interrupt: %s", is_interrupt ? "Yes" : "No");
  print("Interrupt Code: %d (", interrupt_code);

  switch (interrupt_code) {
  case 1: print("Timer");
    break;
  case 9: {
    if (is_interrupt) print("Console");
    else print("ecall in system mode");
    break;
  }
  case 2: print("Illegal instruction");
    break;
  case 5: print("Illegal read address");
    break;
  case 7: print("Illegal write address");
    break;
  case 8: print("ecall in user mode");
    break;
  default: print("Unknown");
  }
  println(")");

}
