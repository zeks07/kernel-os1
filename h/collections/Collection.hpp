#pragma once


namespace collection {
  template <typename T>
  class Collection {
  public:
    virtual auto add(const T& element) -> void = 0;
    virtual auto add(int index, T& element) -> void = 0;
    virtual auto operator[](int index) const -> T& = 0;
    virtual auto get(int index) const -> T& = 0;
    virtual auto remove_at(int index) -> T = 0;
    virtual auto length() const -> int = 0;
    virtual auto is_empty() const -> bool = 0;
    virtual auto clear() -> void = 0;
    virtual ~Collection() = default;
  };
}