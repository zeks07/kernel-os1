#pragma once


namespace kernel::console {
  class Console {
    public
    :
    static auto handle() -> void;
    static auto getc() -> char;
    static auto putc(char chr) -> void;
  };
}
