#ifndef QUEUE_H_
#define QUEUE_H_

#include <list>
#include <stdexcept>

template <typename T>
class Queue {
public:

 // Return number of items in queue
    size_t Size() {
        return items.size();
    }
    // Return front of queue
    T& Front() {
        if (!items.size())
            throw std::underflow_error("Empty queue!");
        return items.front();
    }
    // Remove front of queue
    void Pop() {
        if (!items.size())
            throw std::underflow_error("Empty queue!");
        items.pop_front();
    }
    // Push item to back of queue
    void Push(const T& item) {
        items.push_back(item);
    }

private:
    std::list<T> items;
};



#endif  // QUEUE_H_
