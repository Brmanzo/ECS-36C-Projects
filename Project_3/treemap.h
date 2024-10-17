#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <stack>
#include <utility>

template <typename K, typename V>
class Treemap {
 public:
  //
  // @@@ The class's public API below should _not_ be modified @@@
  //

  // * Capacity
  // Returns number of key-value mappings in map --O(1)
  size_t Size();

  // Returns true if map is empty --O(1)
  bool Empty();

  // * Modifiers
  // Inserts @key in map --O(log N) on average
  // Throws exception if key already exists
  void Insert(const K& key, const V& value);

  // Remove @key from map --O(log N) on average
  // Throws exception if key doesn't exists
  void Remove(const K& key);

  // * Lookup
  // Return value corresponding to @key --O(log N) on average
  // Throws exception if tree is empty or key doesn't exists
  const V& Get(const K& key);

  // Return greatest key less than or equal to @key --O(log N) on average
  // Throws exception if tree is empty or no floor exists for key
  const K& FloorKey(const K& key);

  // Return least key greater than or equal to @key --O(log N) on average
  // Throws exception if tree is empty or no ceil exists for key
  const K& CeilKey(const K& key);

  // Return whether @key is found in map --O(log N) on average
  bool ContainsKey(const K& key);

  // Return whether @value is found in map --O(N)
  bool ContainsValue(const V& value);

  // Return max key in map --O(log N) on average
  // Throws exception if tree is empty
  const K& MaxKey();

  // Return min key in map --O(log N) on average
  // Throws exception if tree is empty
  const K& MinKey();

 private:
  //
  // @@@ The class's internal members below can be modified @@@
  //

  // Private member variables
  // ...To be completed...
  struct Node {
    K key;
    V value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
  };
  std::unique_ptr<Node> root;
  size_t cur_size = 0;
  // Private constants
  // ...To be completed (if any)...

  // Private methods
  // ...To be completed (if any)...
  Node *Min(Node *n);

  Node *Max(Node *n);

  void Insert(std::unique_ptr<Node>& n, const K& key, const V& value);

  void Remove(std::unique_ptr<Node>& n, const K& key);
};

//
// Your implementation of the class should be located below
//
// ...To be completed...
template <typename K, typename V>
size_t Treemap<K, V>::Size() {
  return cur_size;
}
template <typename K, typename V>
bool Treemap<K, V>::Empty() {
  return cur_size == 0;
}

template <typename K, typename V>
void Treemap<K, V>::Insert(const K& key, const V& value) {
  Insert(root, key, value);
}

template <typename K, typename V>
void Treemap<K, V>::Insert(std::unique_ptr<Node>& n,
  const K& key, const V& value) {
  // If node doesnt exist in tree, insert the node
  if (!n) {
    n = std::unique_ptr<Node>(new Node{ key, value });
    cur_size++;
  // If input key is less than node, move left
  } else if (key < n->key) {
    Insert(n->left, key, value);
  // If input key is greater than node, move right
  } else if (key > n->key) {
    Insert(n->right, key, value);
  // Otherwise (key == node) throw error
  } else {
    throw std::invalid_argument("Duplicate Key");
  }
}

template <typename K, typename V>
void Treemap<K, V>::Remove(const K& key) {
  Remove(root, key);
}

template <typename K, typename V>
void Treemap<K, V>::Remove(std::unique_ptr<Node>& n, const K& key) {
  // If key not found, throw error
  if (!n)
    throw std::invalid_argument("Invalid  key");

  // If input key is less than node move left
  if (key < n->key) {
    Remove(n->left, key);
  // If input key is greater than node move right
  } else if (key > n->key) {
    Remove(n->right, key);
  // Otherwise, we have found the node to remove
  } else {
    if (n->left && n->right) {
    // If two children: replace with min node in right subtree
      n->key = Min(n->right.get())->key;
      Remove(n->right, n->key);
    } else {
    // If remaining node is on right, move to left
       n = std::move((n->left) ? n->left : n->right);
       cur_size--;
      }
  }
}
template <typename K, typename V>
const V& Treemap<K, V>::Get(const K& key) {
  if (Empty()) {
    throw std::underflow_error("Empty tree");
  }
  Node *n = root.get();
  while (n) {
    // Found the node to get
    if (key == n->key)
      return n->value;
    // If input key is less than node, move left
    if (key < n->key)
      n = n->left.get();
    // Otherwise, input key is greater than node and moves right
    else
      n = n->right.get();
  }
  throw std::invalid_argument("Invalid  key");
}

template <typename K, typename V>
const K& Treemap<K, V>::FloorKey(const K& key) {
  // n and floor are both set to point to root
  Node *n = root.get();
  Node *floor = root.get();
  if (Empty())
    throw std::underflow_error("Empty tree");
  if (key < MinKey())
    throw std::out_of_range("Out of range!");
  while (n) {
    // If node is == to key, floor is found
    if (key == n->key) {
      return n->key;
    }
    // If node is less than the floor
    // Current node becomes the new floor
    if (n->key < floor->key) {
        floor = n;
    }
    // If key is less than node, move left
    if (key < n->key) {
      n = n->left.get();
    // Otherwise, if key is greater than node,
    // move right and set set floor equal to
    // the current node
    } else {
        floor = n;
        n = n->right.get();
      }
  }
  return floor->key;
}

template <typename K, typename V>
const K& Treemap<K, V>::CeilKey(const K& key) {
  if (Empty())
    throw std::underflow_error("Empty tree");
  Node *n = root.get();
  Node *ceil = root.get();
  if (key > MaxKey())
    throw std::out_of_range("Out of range!");
  while (n) {
      // If node is == to key, ceiling is found
    if (key == n->key)
      return n->key;
    // If node is greater than the ceiling
    // Current node becomes the new ceiling
    if (n->key > ceil->key)
        ceil = n;
    // If key is greater than node, move right
    if (key > n->key) {
      n = n->right.get();
    // Otherwise, if key is less than node,
    // move left and set set ceiling equal to
    // the current node
    } else {
        ceil = n;
        n = n->left.get();
    }
  }
  return ceil->key;
}

template <typename K, typename V>
bool Treemap<K, V>::ContainsKey(const K& key) {
  Node *n = root.get();

  while (n) {
    // If key == node, key exists
    if (key == n->key)
      return true;
    // If key is less than node, move left
    if (key < n->key)
      n = n->left.get();
    // Otherwise, if key is greater than node, move right
    else
      n = n->right.get();
  }
  return false;
}

template <typename K, typename V>
bool Treemap<K, V>::ContainsValue(const V& value) {
  std::queue<Node*> queue;
  queue.push(root.get());
  while (!queue.empty()) {
    // Reads from the front of the queue
    Node *n = queue.front();
    queue.pop();
    // If the node's value == input value, value is found
    if (n->value == value) {
      return true;
    // Otherwise push left and right children onto queue
    } else {
      if (n->left) {
        queue.push(n->left.get());
      }
      if (n->right) {
        queue.push(n->right.get());
      }
    }
  }
  return false;
}

// Private Helper Functions
template <typename K, typename V>
const K& Treemap<K, V>::MaxKey() {
  if (Empty())
    throw std::underflow_error("Empty tree");
  return Max(root.get())->key;
}

template <typename K, typename V>
typename Treemap<K, V>::Node* Treemap<K, V>::Max(Node *n) {
  if (n->right)
    return Max(n->right.get());
  else
    return n;
}

template <typename K, typename V>
const K& Treemap<K, V>::MinKey() {
  if (Empty()) {
    throw std::underflow_error("Empty tree");
  }
  return Min(root.get())->key;
}

template <typename K, typename V>
typename Treemap<K, V>::Node* Treemap<K, V>::Min(Node *n) {
  if (n->left)
    return Min(n->left.get());
  else
    return n;
}

#endif  // TREEMAP_H_
