#pragma once
#include <map>
#include <memory>
#include <type_traits>

namespace smartcompleter {

template <class CharT, class T>
class Trie {
 public:
  static_assert(std::is_move_constructible<T>::value,
                "T must be MoveConstructible");

  struct Node {
    std::map<CharT, std::unique_ptr<Node>> edges;
    Node* parent = nullptr;
    std::unique_ptr<T> data_ptr = nullptr;
  };
  std::unique_ptr<Node> root_ptr_;

  Trie() : root_ptr_(std::make_unique<Node>()) {}

  template <class InputIterator>
  void InsertOrAssign(InputIterator begin, InputIterator end, T value) {
    InsertOrAssign(begin, end, std::move(value), root_ptr_.get());
  }

  template <class InputIterator>
  T* Get(InputIterator begin, InputIterator end) const {
    Node* current_node = root_ptr_.get();
    for (auto it = begin; it != end; ++it) {
      if (auto edge_it = current_node->edges.find(*it);
          edge_it == current_node->edges.end())
        return nullptr;
      else
        current_node = edge_it->second.get();
    }
    return current_node->data_ptr.get();
  }

 protected:
  template <class InputIterator>
  void InsertOrAssign(InputIterator begin, InputIterator end, T value,
                      Node* current_node) {
    if (begin == end) {
      current_node->data_ptr = std::make_unique<T>(std::move(value));
      return;
    }
    if (auto edge_it = current_node->edges.find(*begin);
        edge_it == current_node->edges.end()) {
      auto next_node = std::make_unique<Node>();
      next_node->parent = current_node;
      current_node = next_node.get();
      current_node->edges.emplace(*begin, std::move(next_node));
    } else {
      current_node = edge_it->second.get();
    }
    InsertOrAssign(++begin, end, std::move(value), current_node);
    // TODO: find the most popular word in the subtree during "stack unwinding"
    // Possible implementation: hook-based system for
    // insertion/modification/retrieval/removal (?)
  }
};

}  // namespace smartcompleter
