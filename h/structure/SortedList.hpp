#pragma once

#include "../print/print.hpp"
#include "LinkedNode.hpp"
#include "Pair.hpp"


auto error() -> void;

namespace util {

  template <typename Key>
  auto compare(const Key a, const Key b) -> bool {
    return a < b;
  }

  template <typename T, typename Key, auto (* compare)(Key, Key) -> bool = compare>
  class SortedList {
  public:
    SortedList() : front(nullptr), back(nullptr), size(0) {
    }

    SortedList(const SortedList& other) : front(nullptr), back(nullptr), size(0) {
      auto current = other.front;
      while (current) {
        add(current->get_value().first, current->get_value().second);
        current = current->next_node();
      }
    }

    auto operator=(const SortedList& other) -> SortedList& {
      if (this == &other) return *this;

      clear();

      auto current = other.front;
      while (current) {
        add(current->get_value().first, current->get_value().second);
        current = current->next_node();
      }

      return *this;
    }

    auto add(const T& element, const Key& key) -> void {
      const auto new_node = new LinkedNode<Pair<T, Key>>(Pair<T, Key>(element, key));
      if (is_empty()) {
        front = back = new_node;
      } else {
        auto position = get_position(key);
        if (position) {
          new_node->link_before(position);
          if (position == front) {
            front = new_node;
          }
        } else {
          new_node->link_after(back);
          back = new_node;
        }
      }
      size++;
    }

    auto operator[](const int index) const -> T& {
      return get(index);
    }

    auto get(const int index) const -> T& {
      if (index < 0 || index >= size) {
        println("ERROR: Index out of bounds.");
        error();
      }

      auto current = front;
      for (auto i = 0; i < index; i++) {
        current = current->next_node();
      }

      return current->get_value().get_first();
    }

    auto get_key(const int index) const -> Key& {
      if (index < 0 || index >= size) {
        println("ERROR: Index out of bounds.");
        error();
      }

      auto current = front;
      for (auto i = 0; i < index; i++) {
        current = current->next_node();
      }

      return current->get_value().get_second();
    }

    auto first() const -> T& {
      return get(0);
    }

    auto last() const -> T& {
      return get(size - 1);
    }

    auto remove_at(const int index) -> T {
      if (index < 0 || index >= size) {
        println("ERROR: Index out of bounds.");
        error();
      }

      auto current = front;
      for (auto i = 0; i < index; i++) {
        current = current->next_node();
      }

      auto removed = current->get_value().get_first();

      if (current == front) front = current->next_node();
      if (current == back) back = current->previous_node();

      current->unlink();

      delete current;
      size--;

      return removed;
    }

    auto remove_first() -> T {
      return remove_at(0);
    }

    auto remove_last() -> T {
      return remove_at(size - 1);
    }

    auto length() const -> int {
      return size;
    }

    auto is_empty() const -> bool {
      return size == 0;
    }

    auto clear() -> void {
      while (!is_empty()) {
        remove_first();
      }
    }

    ~SortedList() {
      clear();
    }

  private:
    using sorted_list_node = LinkedNode<Pair<T, Key>>*;

    auto get_position(const Key& key) -> sorted_list_node {
      auto current = front;
      while (current) {
        if (compare(key, current->get_value().get_second())) {
          return current;
        }
        current = current->next_node();
      }
      return nullptr;
    }
    
    sorted_list_node front;
    sorted_list_node back;
    int size;
  };
}
