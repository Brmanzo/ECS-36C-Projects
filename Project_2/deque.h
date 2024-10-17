#ifndef DEQUE_H_
#define DEQUE_H_

#include <iostream>
#include <cstddef>
#include <algorithm>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <utility>

// Bradley Manzo
// 916953788
// ECS 36C Prof. Porquet
// Spring 2022

template<typename T>
class Deque {
 public:
    //
    // @@@ The class's public API below should NOT be modified @@@
    //

    // Constructor
    Deque() : items(std::unique_ptr<T[]>(new T[capacity])) { }
    // Destructor
    ~Deque() = default;
    //
    // Capacity
    //

    // Return true if empty, false otherwise
    // Complexity: O(1)
    bool Empty() const noexcept {
        if (cur_size == 0) {
            return true;
        } else {
            return false;
        }
    }
    // Return number of items in deque
    // Complexity: O(1)
    size_t Size() const noexcept {
        return cur_size;
    }
    // Resize internal data structure to fit precisely
    // the number of items and free unused memory
    // Complexity: O(N)
    void ShrinkToFit() {
        int size = cur_size;
        Resize(size);
    }
    //
    // Element access
    //

    // Return item at pos @pos
    // Complexity: O(1)
    T& operator[](size_t pos) {
        if (pos >= cur_size) {
            throw std::out_of_range("Out of range!");
        } else {
            // Recieves potition in memory and returns
            // position in deque by adding from head
            // and accounting for the edge case
            return items[(front + pos) % capacity];
        }
    }
    // Return item at front of deque
    // Complexity: O(1)
    T& Front() {
        return items[front];
    }
    // Return item at back of deque
    // Complexity: O(1)
    T& Back() {
        return items[back];
    }
    //
    // Modifiers
    //

    // Clear contents of deque (make it empty)
    // Complexity: O(1)
    void Clear(void) noexcept {
        front = -1;
        back = -1;
        cur_size = 0;
    }

    // Push item @value at front of deque
    // Complexity: O(1) amortized
    // Travels Left
    void PushFront(const T& value) {
        // If full, allocate more space by resizing
        if (capacity == cur_size) {
            Resize(capacity * 2);
        }
        int cap = capacity;
        // If empty, move both front and back to point
        // to the only item in the array (from -1 to 0)
        if (Empty()) {
            front = 0;
            back = 0;
        // Otherwise if front is anywhere from 1 to cap
        // front is decremented to move to the left
        } else if (front > 0) {
            front--;
        // Otherwise front is at zero and must be moved
        // to the rightmost position in capacity
        } else if (front == 0) {
            front = cap - 1;
        } else {
            throw std::out_of_range("Out of range!");
        }
        // Value inserted at new position and current
        // size is updated accordingly
        items[front] = value;
        cur_size++;
    }
    // Push item @value at back of deque
    // Complexity: O(1) amortized
    // Travels right
    void PushBack(const T& value) {
        // If full, allocate more space by resizing
        if (capacity == cur_size) {
            Resize(capacity * 2);
        }
        int cap = capacity;
        // If empty, move both front and back to point
        // to the only item in the array (from -1 to 0)
        if (Empty()) {
            front = 0;
            back = 0;
        // Otherwise if back is between 0 and cap
        // back is incremented to move to the right
        } else if (back < cap) {
            back++;
        // Otherwise, back is at cap and must be moved to
        // the leftmost position in capacity at position 0
        } else {
            back = 0;
        }
        // Value inserted at new position and current
        // size is updated accordingly
        items[back] = value;
        cur_size++;
    }
    // Remove item at front of deque
    // Complexity: O(1) amortized
    // Travels Right
    void PopFront() {
        int cap = capacity;
        // If deque is empty, nothing can be popped so
        // out_of_range is thrown
        if (Empty()) {
            throw std::out_of_range("Deque Empty!");
        // If back and front are equal, they are pointing to
        // the only item in the deque, so popping the item
        // would make the deque empty, and reset the front and back
        } else if (back == front) {
            front = -1;
            back = -1;
        // Otherwise if front is greater than zero and not
        // at cap, front is incremented to the right
        } else if (front < cap - 1 && front >= 0) {
            front++;
        // Otherwise, front is at the cap and must be moved to
        // the leftmost position in capacity at position 0
        } else {
            front = 0;
        }
        // Current size is updated accordingly
        cur_size--;
        // deque shrinks dynamically if current size
        // is a quarter of the allocated memory
        if (cur_size < capacity / 4) {
            Resize(capacity / 2);
        }
    }
    // Remove item at back of deque
    // Complexity: O(1) amortized
    // Travels Left
    void PopBack() {
        int cap = capacity;
        // If deque is empty, nothing can be popped so
        // out_of_range is thrown
        if (Empty()) {
            throw std::out_of_range("Deque Empty!");
        // If back and front are equal, they are pointing to
        // the only item in the deque, so popping the item
        // would make the deque empty, and reset the front and back
        } else if (back == front) {
            front = -1;
            back = -1;
        // Otherwise if back is greater than zero and within range
        // back is decremented to the left
        } else if (back > 0 && back < cap) {
            back--;
        // Otherwise if back is at the left side, it must
        // be moved to the rightmost position in capacity
        } else {
            back = cap - 1;
        }
        // Current size is updated accordingly
        cur_size--;
        // deque shrinks dynamically if current size
        // is a quarter of the allocated memory
        if (cur_size < capacity / 4) {
            Resize(capacity / 2);
        }
    }

 private:
    //
    // @@@ The class's internal members below can be modified @@@
    //

    // Private member variables
    // Initial capacity of 3 items
    size_t capacity = 3;
    std::unique_ptr<T[]> items;
    size_t cur_size = 0;

    int front = -1;
    int back = -1;

    // Private constants
    // ...To be completed (if any)...

    // Private methods
    // ...To be completed (if any)...
    void Resize(size_t new_cap) {
        assert(new_cap && new_cap >= cur_size);
        // If the deque is contiguous
        if (back > front) {
            std::unique_ptr<T[]> new_items(new T[new_cap]);
            // Move will take an amount of elements from the first
            // argument (the beginning of items[]), to the second
            // (the end of items[]), and move the contents into
            // the third argument (a new deque of twice the size)
            std::move(std::next(items.get(), front),
                std::next(items.get(), back + 1),
                new_items.get());
            // Ownership of the unique pointer deques are swapped
            std::swap(items, new_items);
            // Capacity, front, and back are reset
            capacity = new_cap;
            front = 0;
            back = cur_size - 1;
        } else {
            // Otherwise, if the deque is nonontiguous
            std::unique_ptr<T[]> new_items(new T[new_cap]);
            // The portion on the right side is moved to
            // the front of the new deque
            std::move(std::next(items.get(), front),
                std::next(items.get(), capacity),
                new_items.get());
            // The portion on the left side is moved behind
            // the right portion within the new deque
            std::move(std::next(items.get(), 0),
                std::next(items.get(), back + 1),
                std::next(new_items.get(), capacity - front));
            // Ownership of the unique pointer deques are swapped
            std::swap(items, new_items);
            // Capacity, front, and back are reset
            capacity = new_cap;
            front = 0;
            back = cur_size - 1;
        }
    }
};
//
// Your implementation of the class should be located below
//

// Inspiration from https://www.youtube.com/watch?v=WJres9mgiAk
// Inspired by Prof's list_dynamic_array.h program on canvas

#endif  // DEQUE_H_
