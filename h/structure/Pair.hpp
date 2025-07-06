#pragma once


namespace util {
  template <typename First, typename Second>
  class Pair {
  public:
    Pair(const First& first, const Second& second) : first(first), second(second) {
    }

    Pair(const Pair& other) : first(other.first), second(other.second) {
    }

    auto operator=(const Pair& other) -> Pair& = default;

    auto get_first() -> First& {
      return first;
    }

    auto get_first() const -> const First& {
      return first;
    }

    auto get_second() -> Second& {
      return second;
    }

    auto get_second() const -> const Second& {
      return second;
    }

  private:
    First first;
    Second second;
  };
}
