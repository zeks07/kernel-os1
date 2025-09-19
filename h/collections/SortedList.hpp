#pragma once

#include "LinkedNode.hpp"
#include "Pair.hpp"
#include "../kernel.hpp"


namespace collection {
  template <typename Key>
  constexpr auto default_compare(const Key a, const Key b) -> bool {
    return a < b;
  }

  template <typename T, typename Key, auto (* compare)(Key, Key) -> bool = default_compare>
  class SortedList final {
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

    /**
     * Adds a new [element] with the associated [key] to the sorted list.
     * The list maintains its sorted order by inserting the new [element]
     * in the appropriate position based on the [key].
     */
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

    /**
     * Returns the element at the given [index] in the list.
     */
    auto operator[](const int index) const -> T& {
      return get(index);
    }

    /**
     * Returns the element at the given [index] in the list.
     */
    auto get(const int index) const -> T& {
      if (index < 0 || index >= size) kernel::throw_error("ERROR: Index out of bounds.");

      auto current = front;
      for (auto i = 0; i < index; i++) {
        current = current->next_node();
      }

      return current->get_value().get_first();
    }

    /**
     * Returns the key associated with the element at the given [index] in the list.
     */
    auto get_key(const int index) const -> Key& {
      if (index < 0 || index >= size) kernel::throw_error("ERROR: Index out of bounds.");

      auto current = front;
      for (auto i = 0; i < index; i++) {
        current = current->next_node();
      }

      return current->get_value().get_second();
    }

    /**
     * Returns the first element in the list.
     */
    auto first() const -> T& {
      return get(0);
    }

    /**
     * Returns the last element in the list.
     */
    auto last() const -> T& {
      return get(size - 1);
    }

    /**
     * Removes and returns the element at the specified [index] in the list.
     */
    auto remove_at(const int index) -> T {
      if (index < 0 || index >= size) kernel::throw_error("ERROR: Index out of bounds.");

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

    /**
     * Removes and returns the first element in the list
     */
    auto remove_first() -> T {
      return remove_at(0);
    }

    /**
     * Removes and returns the last element in the list
     */
    auto remove_last() -> T {
      return remove_at(size - 1);
    }

    /**
     * Returns the length of the list.
     */
    auto length() const -> int {
      return size;
    }

    /**
     * Return true if the list is empty.
     */
    auto is_empty() const -> bool {
      return size == 0;
    }

    /**
     * Removes all elements from the list.
     */
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
