#pragma once

#include "Node.hpp"
#include "../print/print.hpp"


namespace struc {
  template <typename T>
  class List {
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

    auto add(const T& element) -> void {
      const auto new_node = new LinkedNode<T>(element);
      if (is_empty()) {
        front = back = new_node;
      } else {
        new_node->link_after(back);
        back = new_node;
      }
      size++;
    }

    auto add(const int index, T& element) -> void {
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

      for (int i = 0; i < index; i++) {
        current = current->next_node();
      }
      new_node->link_before(current);
      if (index == 0) front = new_node;
      size++;
    }

    auto add_first(const T& element) -> void {
      add(0, element);
    }

    auto operator[](const int index) const -> T& {
      return get(index);
    }

    auto get(const int index) const -> T& {
      if (index < 0 || index >= size) {
        println("ERROR: Index out of bounds; Program will continue without removing an element.");
        return nullptr;
      }

      auto current = front;
      for (int i = 0; i < index; i++) {
        current = current->next_node();
      }

      return current->get_value();
    }

    auto first() const -> T& {
      return get(0);
    }

    auto last() const -> T& {
      return get(size - 1);
    }

    auto remove_at(const int index) -> T {
      if (index < 0 || index >= size) {
        println("ERROR: Index out of bounds; Program will continue without removing an element.");
        return nullptr;
      }

      auto current = front;
      for (int i = 0; i < index; i++) {
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

    ~List() {
      clear();
    }

  private:
    LinkedNode<T>* front;
    LinkedNode<T>* back;
    int size;
  };

  template <typename T>
  auto empty_list() -> List<T> {
    return List<T>();
  }
}
