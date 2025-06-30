#include "../../h/print/print.hpp"

#include "../../lib/hw.h"
#include "../../lib/console.h"

extern "C" {
  typedef __builtin_va_list va_list;
#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
}

auto print_(const long& value, const int base, const int sign) -> void {
  constexpr char digits[] = "0123456789ABCDEF";
  char buffer[32];
  int neg = 0;
  uint x;
  if (sign && value < 0) {
    neg = 1;
    x = -value;
  } else {
    x = value;
  }
  int i = 0;
  do {
    buffer[i++] = digits[x % base];
  } while ((x /= base) > 0);

  if (neg) buffer[i++] = '-';

  while (--i >= 0) __putc(buffer[i]);
}

auto print_(const char* string) -> void {
  while (*string) __putc(*string++);
}

auto specifier(const char specifier, const va_list& args) -> void {
  if (specifier == 'd') {
    const int value = va_arg(args, int);
    print_(value, 10, 1);
  } else if (specifier == 'u') {
    const int value = va_arg(args, int);
    print_(value, 10, 0);
  } else if (specifier == 'x') {
    const int value = va_arg(args, int);
    print_(value, 16, 0);
  } else if (specifier == 's') {
    const char* value = va_arg(args, char*);
    print_(value);
  } else if (specifier == 'c') {
    const char value = va_arg(args, int);
    __putc(value);
  } else {
    __putc('%');
    __putc(specifier);
  }
}

auto print(const char* format, ...) -> void {
  va_list args;
  va_start(args, format);

  for (const char* c = format; *c; ++c) {
    if (*c == '%') {
      ++c;
      specifier(*c, args);
    }
    else {
      __putc(*c);
    }
  }

  va_end(args);
}

auto println(const char* format, ...) -> void {
  print(format);
  __putc('\n');
}

auto println() -> void {
  __putc('\n');
}
