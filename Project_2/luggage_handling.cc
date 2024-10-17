#ifndef LUGGAGE_HANDLING_CC_
#define LUGGAGE_HANDLING_CC_
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "queue.h"
#include "stack.h"

// Bradley Manzo
// 916953788
// ECS 36C Prof. Porquet
// Spring 2022

int main(int argc, char* argv[]) {
    // Recognizes if command line input is wrong amount of arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] <<
            " <input_file> <container_size>" << std::endl;
        exit(1);
    }
    std::string lug_filename = argv[1];
    std::string input_num = argv[2];

    // Checks that the input number is actually a number
    for (char const& ch : input_num) {
        if (std::isdigit(ch) == 0 && ch != '-') {
            exit(1);
        }
    }

    // Sets queue bound after program ensures it exists and is a number
    int cont_cap = std::stoi(input_num);
    int bag;

    // Recognizes if container capacity is invalid
    if (cont_cap <= 0) {
        std::cerr << "Error: invalid container size" << std::endl;
        exit(1);
    }
    // Attempts to open file named in argv[1]
    std::ifstream lug_file;
    lug_file.open(lug_filename, std::ifstream::in);

    // Recognizes if luggage file is not found and throws error
    if (lug_file.fail()) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        exit(1);
    }
    // Instantiates Stack and Temporary Queue Objects
    Stack<Queue<int>> Containers;
    Queue<int> To_Load;
    // Loads bags into temporary queue until queue is full
    // Once full, the queue is pushed onto the stack and cleaned
    while (lug_file >> bag) {
        int cont_size = To_Load.Size();
        if (cont_size < cont_cap) {
            To_Load.Push(bag);
        } else {
            Containers.Push(To_Load);
            for (int i = 0; i < cont_size; i++) {
                To_Load.Pop();
            }
            To_Load.Push(bag);
        }
    }
    // Pushes the remainder bags to the stack after exiting file
    Containers.Push(To_Load);
    // Prints out every queue inside of the stack
    while (!Containers.IsEmpty()) {
        Queue<int> To_Unload = Containers.Top();
        int cont_size = To_Unload.Size();
        for (int i = 0; i < cont_size; i++) {
            std::cout << To_Unload.Front() << ' ';
            To_Unload.Pop();
        }
        Containers.Pop();
    }
}
#endif  // LUGGAGE_HANDLING_CC_
