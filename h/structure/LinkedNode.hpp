#pragma once

#include "Node.hpp"


namespace util {
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