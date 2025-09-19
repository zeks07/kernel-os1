#include "../h/console.hpp"

#include "../h/semaphore.hpp"
#include "../h/collections/List.hpp"
#include "../lib/hw.h"

namespace kernel {
  namespace console {
    namespace {
      collection::List<char>* input_buffer;
      collection::List<char>* output_buffer;
      sync::Semaphore* input_character;
      sync::Semaphore* output_character;
    }

    auto init() -> void {
      input_buffer = new collection::List<char>;
      output_buffer = new collection::List<char>;
      input_character = new sync::Semaphore(0);
      output_character = new sync::Semaphore(0);
    }

    auto handle_interrupt() -> void {
      const auto irq = plic_claim();

      if (irq != CONSOLE_IRQ) throw_error("Invalid IRQ");

      volatile auto status = *reinterpret_cast<char*>(CONSOLE_STATUS);
      while (status & CONSOLE_RX_STATUS_BIT) {
        const volatile auto data = *reinterpret_cast<char*>(CONSOLE_RX_DATA);
        input_buffer->add(static_cast<char>(data));
        input_character->signal();
        status = *reinterpret_cast<char*>(CONSOLE_STATUS);
      }

      plic_complete(irq);
    }

    auto get_char() -> char {
      input_character->wait();
      return input_buffer->remove_first();
    }

    auto put_char(const char character) -> void {
      output_buffer->add(character);
      output_character->signal();
    }

    auto output_buffer_not_empty() -> bool {
      return !output_buffer->is_empty();
    }

    auto output_consumer_body(void*) -> void {
      while (true) {
        if (!(*reinterpret_cast<char*>(CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT)) continue;

        output_character->wait();
        const auto character = output_buffer->remove_first();
        *reinterpret_cast<char*>(CONSOLE_TX_DATA) = character;
      }
    }
  }
}