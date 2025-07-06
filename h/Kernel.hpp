#pragma once

namespace kernel {
class Kernel {
  public:
    static auto run() -> void;
    static auto force_shutdown() -> void;

    static auto disable_interrupts() -> void;
    static auto enable_interrupts() -> void;

  private:
    static void init();
    static void shutdown();
  };
} // namespace kernel
