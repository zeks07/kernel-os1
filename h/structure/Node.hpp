#pragma once


namespace struc {
  template <typename T>
  class Node {
  public:
    explicit Node(const T& value) : value(value) {
    }

    Node(const Node& other) : value(other.value) {
    }

    auto operator=(const Node& other) -> Node& {
      value = other.value;
      return *this;
    }

    auto get_value() -> T& {
      return value;
    }

    auto get_value() const -> const T& {
      return value;
    }

  private:
    T value;
  };

  template <typename T>
  class LinkedNode : public Node<T> {
  public:
    explicit LinkedNode(const T& value) : Node<T>(value), next(nullptr), previous(nullptr) {
    }

    auto link_after(LinkedNode* node) -> void {
      if (node->next) {
        node->next->previous = this;
      }
      next = node->next;
      node->next = this;
      previous = node;
    }

    auto link_before(LinkedNode* node) -> void {
      if (node->previous) {
        node->previous->next = this;
      }
      previous = node->previous;
      node->previous = this;
      next = node;
    }

    auto unlink() -> void {
      if (previous) {
        previous->next = next;
      }
      if (next) {
        next->previous = previous;
      }
      next = nullptr;
      previous = nullptr;
    }

    auto next_node() const -> LinkedNode* {
      return next;
    }

    auto previous_node() const -> LinkedNode* {
      return previous;
    }

  private:
    LinkedNode* next;
    LinkedNode* previous;
  };
}
