#ifndef ANITABORG_CC_
#define ANITABORG_CC_

#include <algorithm>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include "treemap.h"

// all: prints all the donors by increasing order of donations
void All(Treemap<int, std::string>& donor_tree) {
    int floor = donor_tree.MinKey();
    int size = donor_tree.Size();
    // Prints cheapest donor
    std::cout << donor_tree.Get(donor_tree.MinKey())
        << " (" << donor_tree.MinKey() << ")" << std::endl;
    // For every donor in file, ceiling of prev donor
    // key + 1 returns the next highest donor
    for (int i = 0; i < size - 1; i++) {
        std::cout << donor_tree.Get(donor_tree.CeilKey(floor + 1))
            << " (" << donor_tree.CeilKey(floor + 1) << ")" << std::endl;
        // floor is adjusted to the current donor key + 1
        floor = donor_tree.CeilKey(floor + 1);
    }
}
// rich : prints the donor who donated the largest amount
void Rich(Treemap<int, std::string>& donor_tree) {
    std::cout << donor_tree.Get(donor_tree.MaxKey())
        << " (" << donor_tree.MaxKey() << ")" << std::endl;
}
// cheap : prints the donor who donated the smallest amount
void Cheap(Treemap<int, std::string>& donor_tree) {
    std::cout << donor_tree.Get(donor_tree.MinKey())
        << " (" << donor_tree.MinKey() << ")" << std::endl;
}
// who amount : prints the donor who donated amount, if any

void Who_Amount(Treemap<int, std::string>& donor_tree, int key) {
    std::cout << donor_tree.Get(key) << " ("
        << key << ")" << std::endl;
}
// who + amount : prints the first donor who donated more than amount, if any

void Who_Plus_Amount(Treemap<int, std::string>& donor_tree, int key) {
    if (donor_tree.ContainsKey(key)) {
        std::cout << donor_tree.Get(donor_tree.CeilKey(key + 1))
            << " (" << donor_tree.CeilKey(key + 1) << ")" << std::endl;
    } else {
        std::cout << "No match" << std::endl;
    }
}
// who - amount : prints the first donor who donated less than amount, if any
void Who_Minus_Amount(Treemap<int, std::string>& donor_tree, int key) {
    if (donor_tree.ContainsKey(key)) {
        std::cout << donor_tree.Get(donor_tree.FloorKey(key - 1))
            << " (" << donor_tree.FloorKey(key - 1)
            << ")" << std::endl;
    } else {
        std::cout << "No match" << std::endl;
    }
    std::cout << donor_tree.Get(donor_tree.FloorKey(key - 1))
        << " (" << donor_tree.FloorKey(key - 1) << ")" << std::endl;
}

bool OpenFile(std::string donor_filename,
        Treemap<int, std::string>& donor_tree) {
    std::ifstream donor_file;
    std::string row;
    donor_file.open(donor_filename);
    // If file not found, throw error and return false
    if (donor_file.fail()) {
        std::cerr << "Error: cannot open file wrong_don_file.dat" << std::endl;
        return false;
    }
    while (std::getline(donor_file, row)) {
        // Reads file onto row
        if (row.size() > 0) {
            std::string donor;
            std::string amount_str;
            // Records position of the ',' and final character
            int comma = row.find(',');
            int size = row.size();
            // Iterates through row and appends the
            // amount and donor values to the temp strings
            for (int i = 0; i < comma; i++)
                donor.push_back(row[i]);
            for (int i = comma + 1; i < size; i++)
                amount_str.push_back(row[i]);
            // Amount is converted to int and inserted into tree
            int amount_int = stoi(amount_str);
            donor_tree.Insert(amount_int, donor);
        }
    }
    donor_file.close();
    return true;
}

int main(int argc, char* argv[]) {
    Treemap<int, std::string> donor_tree;
    // 3 arguments means all, rich, or cheap
    if (argc == 3) {
        std::string donor_filename = argv[1];
        std::string input_command = argv[2];

        OpenFile(donor_filename, donor_tree);
        if (input_command == "all") {
            All(donor_tree);
        } else if (input_command == "rich") {
            Rich(donor_tree);
        } else if (input_command == "cheap") {
            Cheap(donor_tree);
        // If who is entered, specify that a fourth argument is required
        } else if (input_command == "who") {
            std::cerr << "Command '" << input_command <<
                "' expects another argument: [+/-]amount" << std::endl;
            exit(1);
        } else {
            std::cerr << "Command '" << input_command << "' is invalid" <<
                std::endl << "Possible commands are: all|cheap|rich|who"
                << std::endl;
            exit(1);
        }
    // 4 arguments means who
    } else if (argc == 4) {
        std::string donor_filename = argv[1];
        std::string input_command = argv[2];
        std::string input_args = argv[3];
        // If fourth arg is an integer
        OpenFile(donor_filename, donor_tree);
        if (isdigit(input_args[0])) {
            int key = stoi(input_args);
            Who_Amount(donor_tree, key);
        // If fourth arg begins with '+'
        } else if (input_args[0] == '+') {
            input_args.erase(remove(input_args.begin(),
                input_args.end(), '+'), input_args.end());
            int key = stoi(input_args);
            Who_Plus_Amount(donor_tree, key);
        // If 4th arg begins with '-'
        } else if (input_args[0] == '-') {
            input_args.erase(remove(input_args.begin(),
                input_args.end(), '-'), input_args.end());
            int key = stoi(input_args);
            Who_Minus_Amount(donor_tree, key);
        } else {
            std::cerr <<
                "Command 'who' expects another argument : [+/ -] amount"
                << std::endl;
            exit(1);
        }
    } else {
        std::cerr
            << "Usage: " << argv[0] <<
            " <donations_file.dat> <command> [<args>]" << std::endl;
        exit(1);
    }
}
#endif  // ANITABORG_CC_
