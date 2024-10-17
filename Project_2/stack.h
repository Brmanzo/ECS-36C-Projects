#ifndef STACK_H_
#define STACK_H_

#include <forward_list>
#include <stdexcept>

template <typename T>
class Stack {
public:
    // Return number of items in stack
    size_t Size() {
        return cur_size;
    }
    // Return top of stack
    T& Top() {
        if (!cur_size)
            throw std::underflow_error("Empty stack!");
        return items.front();
    }
    // Remove top of stack
    void Pop() {
        if (!cur_size)
            throw std::underflow_error("Empty stack!");
        items.pop_front();
        cur_size--;
    }
    // Push item to top of stack
    void Push(const T& item) {
        items.push_front(item);
        cur_size++;
    }
    // Returns true if empty
    bool IsEmpty() {
        if (items.empty())
            return true;
        else
            return false;
    }
private:
    size_t cur_size = 0;
    std::forward_list<T> items;
};



#endif  // STACK_H_
