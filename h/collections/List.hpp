#pragma once

#include "Collection.hpp"
#include "LinkedNode.hpp"
#include "../kernel.hpp"
#include "../print.hpp"


namespace collection {
  template <typename T>
  class List final : public Collection<T> {
  public:
    List() : front(nullptr), back(nullptr), size(0) {
    }

    List(const List& other) : front(nullptr), back(nullptr), size(0) {
      auto current = other.front;
      while (current) {
        add(current->get_value());
        current = current->next_node();
      }
    }

    auto operator=(const List& other) -> List& {
      if (this == &other) return *this;

      clear();

      auto current = other.front;
      while (current) {
        add(current->get_value());
        current = current->next_node();
      }

      return *this;
    }

    /**
     * Adds [element] at the end of the list.
     */
    auto add(const T& element) -> void override {
      const auto new_node = new LinkedNode<T>(element);
      if (is_empty()) {
        front = back = new_node;
      } else {
        new_node->link_after(back);
        back = new_node;
      }
      size++;
    }

    /**
     * Adds [element] at the given [index] in the list.
     */
    auto add(const int index, T& element) -> void override {
      if (index < 0 || index > size) {
        println("ERROR: Index out of bounds; Program will continue without inserting a new element.");
        return;
      }

      if (index == size) {
        add(element);
        return;
      }

      const auto new_node = new LinkedNode<T>(element);
      auto current = front;

      for (auto i = 0; i < index; i++) {
        current = current->next_node();
      }
      new_node->link_before(current);
      if (index == 0) front = new_node;
      size++;
    }

    /**
     * Adds [element] at the beginning of the list.
     */
    auto add_first(const T& element) -> void {
      add(0, element);
    }

    /**
     * Returns the element at the given [index].
     */
    auto operator[](const int index) const -> T& override {
      return get(index);
    }

    /**
     * Returns the element at the given [index].
     */
    auto get(const int index) const -> T& override {
      if (index < 0 || index >= size) kernel::throw_error("ERROR: Index out of bounds.");

      auto current = front;
      for (auto i = 0; i < index; i++) {
        current = current->next_node();
      }

      return current->get_value();
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
    auto remove_at(const int index) -> T override {
      if (index < 0 || index >= size) kernel::throw_error("ERROR: Index out of bounds.");

      auto current = front;
      for (auto i = 0; i < index; i++) {
        current = current->next_node();
      }

      auto removed = current->get_value();

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
    auto length() const -> int override {
      return size;
    }

    /**
     * Return true if the list is empty.
     */
    auto is_empty() const -> bool override {
      return size == 0;
    }

    /**
     * Removes all elements from the list.
     */
    auto clear() -> void  override {
      while (!is_empty()) {
        remove_first();
      }
    }

    ~List() override {
      clear();
    }

  private:
    LinkedNode<T>* front;
    LinkedNode<T>* back;
    int size;
  };
}
