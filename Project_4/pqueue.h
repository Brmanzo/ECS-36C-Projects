#ifndef PQUEUE_H_
#define PQUEUE_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>
#include <stdexcept>
#include <utility>

// Raj Garimella
// 919351073
// Bradley Manzo
// 916953788
// Programming Assignment #4
// Prof. Joel Porquet-Lupine
// May 20th, 2022

template <typename T, typename C = std::less<T>>
class PQueue {
 public:
  // Constructor
  PQueue() {}

  // * Capacity
  // Return number of items in priority queue
  size_t Size();

  // * Element Access
  // Return top of priority queue
  T& Top();

  // * Modifiers
  // Remove top of priority queue
  void Pop();
  // Insert item and sort priority queue
  void Push(const T &item);

 private:
  // Private member variables
  std::vector<T> items;
  size_t cur_size = 0;
  C cmp;

  // Private methods

  // * Helper methods for indexing queue
  // Locates root node
  size_t Root() {
    return 0;
  }
  // Locates respective parent node
  size_t Parent(size_t n) {
    return (n - 1) / 2;
  }
  // Locates respective left child
  size_t LeftChild(size_t n) {
    return 2 * n + 1;
  }
  // Locates respective right child
  size_t RightChild(size_t n) {
    return 2 * n + 2;
  }

  // * Helper methods for testing nodes
  // Checks for parent, (root is the only orphan node)
  bool HasParent(size_t n) {
    return n != Root();
  }
  // Checks if valid position in queue
  bool IsNode(size_t n) {
    return n < cur_size;
  }

  // * Helper methods for restructuring
  // Swaps higher priority node with lower priority
  // ancestor until the node is correctly placed
  void PercolateUp(size_t n);
  // Swaps lower priority node with higher priority
  // descendents until the node is correctly placed
  void PercolateDown(size_t n);

  // Recieves comparator from user and compares two values
  bool CompareNodes(size_t i, size_t j);
};

template <typename T, typename C>
bool PQueue<T, C>::CompareNodes(size_t i, size_t j) {
  return cmp(items[i], items[j]);
}

template <typename T, typename C>
size_t PQueue<T, C>::Size() {
  return cur_size;
}

template <typename T, typename C>
T& PQueue<T, C>::Top() {
  if (!Size())
    throw std::underflow_error("Empty priority queue!");
  return items[Root()];
}

template <typename T, typename C>
void PQueue<T, C>::Push(const T &item) {
  items.push_back(item);
  cur_size++;
  PercolateUp(Size() - 1);
}

template <typename T, typename C>
void PQueue<T, C>::PercolateUp(size_t n) {
    while (HasParent(n) && CompareNodes(n, Parent(n))) {
    std::swap(items[Parent(n)], items[n]);
    n = Parent(n);
  }
}

template <typename T, typename C>
void PQueue<T, C>::Pop() {
  if (!Size())
    throw std::underflow_error("Empty priority queue!");
  // Moves last item in queue to the root
  items[Root()] = items[cur_size - 1];
  items.pop_back();
  cur_size--;
  PercolateDown(Root());
}

template <typename T, typename C>
void PQueue<T, C>::PercolateDown(size_t n) {
  while (IsNode(LeftChild(n))) {
    size_t child = LeftChild(n);

    if (IsNode(RightChild(n)) && CompareNodes(RightChild(n), LeftChild(n)))
      child = RightChild(n);

    if (CompareNodes(child, n))
      std::swap(items[child], items[n]);
    else
      break;

    n = child;
  }
}

// To be completed below

#endif  // PQUEUE_H_
