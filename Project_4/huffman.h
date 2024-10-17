#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <array>
#include <cstddef>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <stack>

#include "bstream.h"
#include "pqueue.h"

class HuffmanNode {
 public:
  explicit HuffmanNode(char ch, size_t freq,
                       HuffmanNode *left = nullptr,
                       HuffmanNode *right = nullptr)
      : ch_(ch), freq_(freq), left_(left), right_(right) { }


  bool IsLeaf() {
    // Node is a leaf if it doesn't have any children
    return left_ == nullptr && right_ == nullptr;
  }

  bool operator < (const HuffmanNode &n) const {
    // In case of equality, make it deterministic based on character
    if (freq_ == n.freq_)
      return ch_ < n.ch_;
    // Otherwise compare frequencies
    return freq_ < n.freq_;
  }

  size_t freq() { return freq_; }
  size_t data() { return ch_; }
  HuffmanNode* left() { return left_; }
  HuffmanNode* right() { return right_; }

 private:
  char ch_;
  size_t freq_;
  HuffmanNode *left_, *right_;
};

// Dereferences the pointers to Huffman Nodes to compare the overloaded
// operator < for the HuffmanNode class rather than the memory addresses.
class HuffmanNodePointerLess {
 public:
  bool operator () (HuffmanNode *n1, HuffmanNode *n2) const {
    return (*n1 < *n2);
  }
};

class Huffman {
 public:
  static void Compress(std::ifstream &ifs, std::ofstream &ofs);

  static void Decompress(std::ifstream &ifs, std::ofstream &ofs);

 private:
  // Helper methods...
  static void RecordFrequencies(std::ifstream &ifs, PQueue<HuffmanNode*,
      HuffmanNodePointerLess> &min_queue, int &num_chars, std::string &input);
  static void MakeHuffmanTree(PQueue<HuffmanNode*,
      HuffmanNodePointerLess> &min_queue);
  static void PreOrderTrav(HuffmanNode *n, std::vector<std::string> &code_table,
      std::string &path, BinaryOutputStream &bos,
      bool is_root, bool is_right_child);
  static void WriteCodeTable(std::vector<std::string> &code_table,
      std::string &input, BinaryOutputStream &bos);

  static HuffmanNode* ReconstructTree(BinaryInputStream &bis);
  static HuffmanNode* CreateNode(BinaryInputStream &bis);
  static int FindLeftMostPath(HuffmanNode *n);
  static void TraverseTree(const int num_chars, HuffmanNode *n,
      BinaryInputStream &bis, std::ofstream &ofs);
  static void DeleteTree(HuffmanNode *n);
};


// To be completed below

void Huffman::Compress(std::ifstream &ifs, std::ofstream &ofs) {
  PQueue<HuffmanNode*, HuffmanNodePointerLess> min_queue;
  int num_chars = 0;
  std::string input = "";
  RecordFrequencies(ifs, min_queue, num_chars, input);
  MakeHuffmanTree(min_queue);
  BinaryOutputStream bos(ofs);
  std::vector<std::string> code_table(128);
  std::string path;
  PreOrderTrav(min_queue.Top(), code_table, path, bos, true, false);
  bos.PutInt(num_chars);
  WriteCodeTable(code_table, input, bos);
  HuffmanNode *root = min_queue.Top();
  min_queue.Pop();
  DeleteTree(root);
}

// Reads through the input. Whenever a character is encountered, its value in
// freqnecies array is incrimented by 1. The index of a char in the array is
// its ASCII value. The number of characters is recoreded and then all the
// characters with their frequencies are pushed to the priority queue.
void Huffman::RecordFrequencies(std::ifstream &ifs, PQueue<HuffmanNode*,
    HuffmanNodePointerLess> &min_queue, int &num_chars, std::string &input) {
  int frequencies[128] = {0};
  char next_char;
  while (ifs.get(next_char)) {
    frequencies[int(next_char)]++;
    input.push_back(next_char);
    num_chars++;
  }

  for (int i = 0; i < 128; ++i) {
    if (frequencies[i] > 0)
      min_queue.Push(new HuffmanNode(i, frequencies[i]));
  }
  ifs.close();
}

// Pops off the first 2 nodes from the min queue of pointers to HuffmanNodes,
// makes them the children of a new node, and pushes the new parent node to
// the priority queue. This is repeated until the prioirity queue only has one
// node pointer in it (the root).
void Huffman::MakeHuffmanTree(PQueue<HuffmanNode*,
    HuffmanNodePointerLess> &min_queue) {
  while (min_queue.Size() > 1) {
    HuffmanNode *left_child(min_queue.Top());
    min_queue.Pop();
    HuffmanNode *right_child(min_queue.Top());
    min_queue.Pop();
    min_queue.Push(new HuffmanNode(0,
        left_child->freq() + right_child->freq(),
        left_child, right_child));
  }
}

// Traverses the tree with pre order traversal. When it encoutners a leaf, it
// outputs a 1 bit followed by the char stored there. When it encounters an
// internal node, it outputs a 0 bit. Additionally, it records the path to
// each node.
void Huffman::PreOrderTrav(HuffmanNode *n,
    std::vector<std::string> &code_table, std::string &path,
    BinaryOutputStream &bos, bool is_root, bool is_right_child) {
  // returns if the node doesn't exist
  if (!n)
    return;

  // Appends a 0 or 1 to the path based whether it is a left or right child.
  if (!is_root) {
    if (is_right_child)
      path.append("1");
    else
      path.append("0");
  }

  // Outputs to BinaryOutputStream based on whether the node is a leaf or not.
  if ((n->IsLeaf())) {
    bos.PutBit(1);
    bos.PutChar((char)n->data());

    // Stores the path to each node to the code table.
    code_table[n->data()] = path;
  } else {
    bos.PutBit(0);
  }
  // Resets the path to what it was after each recrusive call.
  std::string path_to_n = path;
  Huffman::PreOrderTrav(n->left(), code_table, path, bos, false, false);
  path = path_to_n;
  Huffman::PreOrderTrav(n->right(), code_table, path, bos, false, true);
  path = path_to_n;
}

// Iterates through the string containing the input.
// For each char, it finds the string that stores its path in the code table.
// For each char in the path, it ouputs a 0 or 1 bit based on whether it was a
// 1 or a zero.
void Huffman::WriteCodeTable(std::vector<std::string> &code_table,
    std::string &input, BinaryOutputStream &bos) {
  for (size_t i = 0; i < input.length(); ++i) {
    for (size_t j = 0; j < code_table[input[i]].size(); ++j) {
      if (code_table[input[i]][j] == '1')
        bos.PutBit(1);
      else
        bos.PutBit(0);
    }
  }
  bos.Close();
}


void Huffman::Decompress(std::ifstream &ifs, std::ofstream &ofs) {
  BinaryInputStream bis(ifs);
  HuffmanNode* tree = ReconstructTree(bis);
  int num_chars = bis.GetInt();
  // Repeats tree traversal for the number of chars.
  for (int i = 0; i < num_chars; ++i)
    TraverseTree(num_chars, tree, bis, ofs);

  ifs.close();
  ofs.close();
  DeleteTree(tree);
}

// Creates a new HuffmanNode pointer by reading from the BinaryInputStream and
// making an empty node if it reads a 0, and a node containing the next char
// read from the stream if it read a 1.
HuffmanNode* Huffman::CreateNode(BinaryInputStream &bis) {
  bool bit = bis.GetBit();
  if (!bit)
    return new HuffmanNode(0, 0);

  char next = bis.GetChar();
  return new HuffmanNode(next, 0);
}

// Reconstructs the Huffman tree from the bottom up.
HuffmanNode* Huffman::ReconstructTree(BinaryInputStream &bis) {
  std::stack<HuffmanNode*> s;
  // A full node is defined as either a node that stores a char or a parent.
  int num_consecutive_full_nodes = 0;
  HuffmanNode *node = CreateNode(bis);
  // If the first node contains a character, it is the only node so the
  // function returns immediatly.
  if (node->data() != 0)
    return node;

  s.push(node);
  while (s.size() != 1 || s.top()->IsLeaf()) {
    // Pushes new nodes to the stack until there a 2 consecutuve full nodes.
    while (num_consecutive_full_nodes != 2) {
      node = CreateNode(bis);
      s.push(node);

      // If the top is a full node, incriment num_consecutive_full_nodes
      // Otherwise, reset it to 0.
      if (s.top()->data() != 0)
        num_consecutive_full_nodes++;
      else
        num_consecutive_full_nodes = 0;
    }

    // If the top 2 nodes are both full, removes the top 3 and pushes a new
    // node to the stack that has the previous 2 as children.
    if (num_consecutive_full_nodes == 2) {
      // Pops the top 2 and saves them and pops and deletes the top third.
      HuffmanNode* right = s.top();
      s.pop();
      HuffmanNode* left = s.top();
      s.pop();
      HuffmanNode *temp = s.top();
      s.pop();
      // We are deleting this node because we are replacing it in the tree
      // rather than adding it.
      delete temp;

      // if the queue is empty, return the root node
      // rather than pushing it back to the stack.
      if (s.size() == 0)
        return new HuffmanNode(0, 0, left, right);

      // if the top of the stack (after popping out the 3 nodes) is full
      // the num_consecutive_full_nodes is 2 (including the one that is about
      // to be added).
      // Otherwise it is 1 (including the one that is about to be added).
      if (!s.top()->IsLeaf() || s.top()->data() != 0)
        num_consecutive_full_nodes = 2;
      else
        num_consecutive_full_nodes = 1;

      // Pushes the parent to the stack
      s.push(new HuffmanNode(0, 0, left, right));
    }
  }
  return s.top();
}

// Traverses the tree by reading bits from the input and going left when it
// encounters a 0 bit and right when it enocunters a 1 bit. When it reaches a
// node that stores a char, it outputs that char to the output stream.
void Huffman::TraverseTree(const int num_chars,
    HuffmanNode *n, BinaryInputStream &bis, std::ofstream &ofs) {
  char character;
  while (n->data() == 0) {
    bool bit = bis.GetBit();
    if (bit)
      n = n->right();
    else
      n = n->left();
}
  character = n->data();
  ofs.put(character);
}

// Uses postorder traversal to delete the tree, freeing the memory.
void Huffman::DeleteTree(HuffmanNode *n) {
  if (!n)
    return;

  DeleteTree(n->left());
  DeleteTree(n->right());
  delete n;
}

#endif  // HUFFMAN_H_
