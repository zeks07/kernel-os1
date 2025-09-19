#pragma once


namespace collection {
  template <typename T>
  class Node {
  public:
    explicit Node(const T& value) : value(value) {
    }

    Node(const Node& other) : value(other.value) {
    }

    auto operator=(const Node& other) -> Node& = default;

    auto get_value() -> T& {
      return value;
    }

    auto get_value() const -> const T& {
      return value;
    }

  private:
    T value;
  };
}