#include "../h/Kernel.hpp"

#include "../h/syscall_cpp.hpp"
#include "../h/thread/Scheduler.hpp"
#include "../h/memory/MemoryAllocator.hpp"
#include "../h/time/Timer.hpp"


extern "C" void trap_vector_base();

extern auto userMain() -> void;

namespace {
  constexpr auto direct_mode = 0b00;
}

auto volatile finished = false;

auto user_thread_wrapper(void*) -> void {
  userMain();
  finished = true;
}

auto kernel::Kernel::run() -> void {
  init();

  thread_t kernel_thread;
  thread::create_thread(&kernel_thread, nullptr, nullptr);

  thread::Scheduler::init(kernel_thread);

  Thread user_thread(&user_thread_wrapper, nullptr);
  user_thread.start();

  while (!finished) {
    thread_dispatch();
  }

  shutdown();
}

auto kernel::Kernel::force_shutdown() -> void {
  shutdown();
}

auto kernel::Kernel::disable_interrupts() -> void {
  __asm__ volatile("li t0, 0b10");
  __asm__ volatile("csrc sip, t0");
}

auto kernel::Kernel::enable_interrupts() -> void {
  __asm__ volatile("li t0, 0b10");
  __asm__ volatile("csrs sip, t0");
}

auto kernel::Kernel::init() -> void {
  auto trap_vector_base_pointer = reinterpret_cast<uint64>(&trap_vector_base) | direct_mode;
  __asm__ volatile("csrw stvec, %0" : : "r"(trap_vector_base_pointer));
  MemoryAllocator::init();
  time::Timer::init();
}

void kernel::Kernel::shutdown() {
  thread::Scheduler::destructor();
  __asm__ volatile ("li t0, 0x5555");
  __asm__ volatile ("li t1, 0x100000");
  __asm__ volatile ("sw t0, 0(t1)");
}
