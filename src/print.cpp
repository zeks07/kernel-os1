#include "../h/print.hpp"

#include "../h/syscall_c.h"

extern "C" {
  typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
}

namespace {
  constexpr char digits[] = "0123456789ABCDEF";

  auto print_number(const long& value, const int base, const int sign) -> void {
    char buffer[32];
    auto negative = 0;
    unsigned int x;
    if (sign && value < 0) {
      negative = 1;
      x = -value;
    } else {
      x = value;
    }
    auto i = 0;
    do {
      buffer[i++] = digits[x % base];
    } while ((x /= base) > 0);

    if (negative) buffer[i++] = '-';

    while (--i >= 0) putc(buffer[i]);
  }

  auto print_string(const char* string) -> void {
    while (*string) putc(*string++);
  }

  auto specifier(const char specifier, va_list& args) -> void {
    switch (specifier) {
      case 'd': {
        const auto value = va_arg(args, int);
        print_number(value, 10, 1);
        break;
      }
      case 'u': {
        const auto value = va_arg(args, unsigned int);
        print_number(value, 10, 0);
        break;
      }
      case 'x': {
        const auto value = va_arg(args, unsigned int);
        print_number(value, 16, 0);
        break;
      }
      case 's': {
        const auto value = va_arg(args, char*);
        print_string(value);
        break;
      }
      case 'c': {
        const auto value = va_arg(args, int);
        putc(value);
        break;
      }
      default: {
        putc('%');
        putc(specifier);
        break;
      }
    }
  }
}

auto print(const char* format, ...) -> void {
  va_list args;
  va_start(args, format);

  for (auto c = format; *c; ++c) {
    if (*c == '%') {
      ++c;
      specifier(*c, args);
    } else {
      putc(*c);
    }
  }

  va_end(args);
}

auto println(const char* format, ...) -> void {
  print(format);
  putc('\n');
}

auto println() -> void {
  putc('\n');
}