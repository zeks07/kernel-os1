#pragma once

namespace kernel {
  namespace console {
    auto init() -> void;
    auto handle_interrupt() -> void;
    auto get_char() -> char;
    auto put_char(char character) -> void;
    auto output_buffer_not_empty() -> bool;
    [[noreturn]]
    auto output_consumer_body(void*) -> void;
  }
}