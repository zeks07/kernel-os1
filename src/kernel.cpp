#include "../h/kernel.hpp"

#include "../h/console.hpp"
#include "../lib/hw.h"
#include "../h/memory_allocator.hpp"
#include "../h/thread.hpp"
#include "../h/print.hpp"
#include "../h/syscall_cpp.hpp"

extern "C" auto trap_vector_base() -> void;
extern auto userMain() -> void;

namespace kernel {
  namespace {
    constexpr auto direct_mode = 0b00;

    auto volatile finished = false;

    auto user_thread_wrapper(void*) -> void {
      userMain();
      finished = true;
    }

    auto init() -> void
    {
      auto trap_vector_base_pointer = reinterpret_cast<uint64>(&trap_vector_base) | direct_mode;
      __asm__ volatile("csrw stvec, %0" : : "r"(trap_vector_base_pointer));

      memory::init();
      thread::init();
      console::init();

      __asm__ volatile("li t0, 0x2");
      __asm__ volatile("csrs sstatus, t0");
    }
  }

  auto run() -> void {
    init();

    Thread output_consumer(&console::output_consumer_body, nullptr);
    output_consumer.start();

    Thread user_thread(&user_thread_wrapper, nullptr);
    user_thread.start();

    while (!finished) {
      thread_dispatch();
    }

    while (console::output_buffer_not_empty()) {
      thread_dispatch();
    }

    shutdown();
  }

  auto shutdown() -> void {
    __asm__ volatile ("li t0, 0x5555");
    __asm__ volatile ("li t1, 0x100000");
    __asm__ volatile ("sw t0, 0(t1)");
  }

  auto throw_error(const char* message) -> void {
    println(message);
    shutdown();
  }

  auto error(const uint64 status, const uint64 cause) -> void {
    println("An error has occurred.");

    const auto in_system_mode = (status & 1 << 8) == 1 << 8;
    const auto masked_interrupts = (status & 1 << 1) == 1 << 1;
    const auto previous_interrupt_enabled = (status & 1 << 5) == 1 << 5;

    const auto is_interrupt = (cause & 1 << 31) == static_cast<uint64>(1) << 31;
    const auto interrupt_code = cause & 0x7FFFFFFF;

    println("Status: %s", in_system_mode ? "System" : "User");
    println("Interrupts: %s", masked_interrupts ? "Masked" : "Unmasked");
    println("Previous Interrupts: %s", previous_interrupt_enabled ? "Enabled" : "Disabled");

    println("Interrupt: %s", is_interrupt ? "Yes" : "No");
    print("Interrupt Code: %d (", interrupt_code);

    switch (interrupt_code) {
      case 1:
        print("Timer");
        break;
      case 9: {
        if (is_interrupt)
          print("Console");
        else
          print("ecall in system mode");
        break;
      }
      case 2:
        print("Illegal instruction");
        break;
      case 5:
        print("Illegal read address");
        break;
      case 7:
        print("Illegal write address");
        break;
      case 8:
        print("ecall in user mode");
        break;
      default:
        print("Unknown");
    }
    println(")");

    shutdown();
  }
}