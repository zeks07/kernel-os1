#pragma once

namespace kernel {
class Kernel {
public:
  static auto run() -> void;
  static auto force_shutdown() -> void;

private:
  static void init();
  static void shutdown();
};
} // namespace kernel