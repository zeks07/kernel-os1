#pragma once

namespace kernel {
  auto run() -> void;
  auto shutdown() -> void;
  auto throw_error(const char* message) -> void;
  auto error(unsigned long status, unsigned long cause) -> void;
}