#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono> // NOLINT (build/c++11)

class PosVec {
 public:
    int x, y, mag;
    PosVec(int pos_x, int pos_y, int in_mag) {
        x = pos_x;
        y = pos_y;
        mag = in_mag;
    }
    // Position Vector Class Constructor
    friend std::ostream& operator<< (std::ostream& output,
        const PosVec& position_vector);
    bool operator< (const PosVec& position_vector) {
        if (mag == position_vector.mag) {
            return y < position_vector.y;
        } else {
            return mag < position_vector.mag;
        }
        // < Operator Overload Function for Position Vector Class
        // Enables proper sorting of position-vector vector
    }
};

std::ostream& operator<< (std::ostream& output, const PosVec& position_vector) {
    output << '<' << position_vector.x << "," << position_vector.y
        << '>' << " Magnitude: " << position_vector.mag << std::endl;
    return output;
}
// Ostream Overload Function for Position Vector Class
// Enables simple print out functionality

class Clock {
 public:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;

    Clock() {
        start = std::chrono::high_resolution_clock::now();
    }
    double CurrentTime() {
        end = std::chrono::high_resolution_clock::now();
        double elapsed_us = std::chrono::duration<double,
            std::micro>(end - start).count();
        return(elapsed_us);
    }
};
// Clock Class which starts upon instantiation
// And stops upon calling CurrentTime()

bool OpenPosVecFile(std::string input_pos_vec_filename,
    std::vector<PosVec>& pos_vec_vec_file) {
    std::ifstream pos_vec_file_holder;
    pos_vec_file_holder.open(input_pos_vec_filename);
    // Opens Position Vector File
    std::string row;
    // String inialized to hold each row
    if (pos_vec_file_holder.fail()) {
        std::cerr << "Error: cannot open file wrong_vect_file.dat" << std::endl;
        return false;
        // returns false if vec file not found
    }
    // Recognizes when the failbit is high and throws an error, Inspired by https://thispointer.com/c-how-to-read-a-file-line-by-line-into-a-vector/
    while (std::getline(pos_vec_file_holder, row)) {
        // Reads file onto row
        if (row.size() > 0) {
            std::string x_str = "";
            std::string y_str = "";
            // Temp strings
            int space = row.find(' ');
            int size = row.size();
            // Records position of the ' ' and final character
            for (int i = 0; i < space; i++) {
                x_str.push_back(row[i]);
            }
            for (int i = space + 1; i < size; i++) {
                y_str.push_back(row[i]);
            }
            // Iterates through row and appends the
            // X and Y values to the temp strings
            int x_int = stoi(x_str);
            int y_int = stoi(y_str);
            // Casts temp strings to ints
            float mag = std::sqrt(x_int * x_int + y_int * y_int);
            int mag_int = static_cast<int>(mag);
            // Calculates magnitude of row
            PosVec pos_vec(x_int, y_int, mag_int);
            pos_vec_vec_file.push_back(pos_vec);
            // Instantiates each object and pushes
            // onto position-vector vector container
        }
    }
    pos_vec_file_holder.close();
    return true;
}
bool OpenMagFile(std::string input_mag_filename,
    std::vector<int>& mag_vec_file) {
    std::ifstream mag_file_holder;
    mag_file_holder.open(input_mag_filename);
    // Opens magnitude file
    std::string row;
    // String inialized to hold each row
    if (mag_file_holder.fail()) {
        std::cerr << "Error: cannot open file wrong_mag_file.dat" << std::endl;
        return false;
        // returns false if mag file not found
    }
    // Recognizes when the failbit is high and throws an error, Inspired by https://thispointer.com/c-how-to-read-a-file-line-by-line-into-a-vector/
    while (std::getline(mag_file_holder, row)) {
        if (row.size() > 0) {
            int mag = stoi(row);
            mag_vec_file.push_back(mag);
        }
    }
    // Casts row to an integer and pushes onto magnitude vector container
    mag_file_holder.close();
    return true;
}

int LinSearch(const std::vector<PosVec>& pos_vec_vec,
    const std::vector<int>& mag_vec) {
    int lin_match_count = 0;
    for (std::vector<int>::size_type i = 0; i != mag_vec.size(); i++) {
        // For every magnitude in the magnitude vector
        for (std::vector<PosVec>::size_type j = 0;
            j != pos_vec_vec.size(); j++) {
            // And for every magnitude in the position-vector vector
            if (mag_vec[i] == pos_vec_vec[j].mag) {
                lin_match_count++;
                break;
                // matches are recorded and breaks
                // to avoid duplicate match counts
            }
        }
    }
    return(lin_match_count);
}

int BinSearch(const std::vector<PosVec>& pos_vec_vec,
    const std::vector<int>& mag_vec) {
    int bin_matches = 0;
    for (std::vector<int>::size_type i = 0; i != mag_vec.size(); i++) {
        // For every magnitude in the magnitude vector
        int lower_b = 0, upper_b = pos_vec_vec.size() - 1;
        // Initializes upper and lower bound parameters
        while (lower_b <= upper_b) {
            int mid = lower_b + (upper_b - lower_b) / 2;
            // Calculates the middle of the vector
            if (pos_vec_vec[mid].mag == mag_vec[i]) {
                bin_matches++;
                break;
                // If values match, matches increment and breaks
            }
            if (pos_vec_vec[mid].mag < mag_vec[i]) {
                lower_b = mid + 1;
                // If middle is less than the target, the
                // middle is refined as the new lower bound
            } else {
                upper_b = mid - 1;
                // If middle is greater than the target, the current
                // middle is redefined as the new upper bound
            }
        }
    }

    return bin_matches;
    // Binary search inspirec by Prof's
    // binary_search.cc on canvas
}

int main(int argc, char* argv[]) {
    std::vector<PosVec> pos_vec_vec;
    std::vector<int> mag_vec;
    int correct_choice = 0;
    char choice;
    // Instantiates position-vector and magnitude
    // vectors, Choice records user input
    // correct_choice handles errors

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] <<
            " <vector_file.dat> <magnitude_file.dat> <result_file.dat>"
            << std::endl;
    }
    // Ensures that proper amount of arguments are entered
    char* pos_vec_file = argv[1];
    bool pos_vec_out = OpenPosVecFile(pos_vec_file, pos_vec_vec);
    // Takes first filename and calls the position-vector open file function
    char* mag_file = argv[2];
    bool mag_out = OpenMagFile(mag_file, mag_vec);
    // Takes second filename and calls the magnitude open file function
    char* dat_out = argv[3];
    if (!pos_vec_out || !mag_out || !dat_out) {
        std::cerr << "Usage: " << argv[0] <<
            " <vector_file.dat> <magnitude_file.dat> <result_file.dat>"
            << std::endl;
    }
    while (correct_choice == 0) {
        std::cout << "Choice of search method ([l]inear, [b]inary)?"
            << std::endl;
        std::cin >> choice;
        switch (choice) {
        case 'l': {
            // If user input is l, linear search function is called
            Clock ct1;
            // Constructs clock object starting clock
            int lin_matches = LinSearch(pos_vec_vec, mag_vec);
            double microseconds = ct1.CurrentTime();
            // Computes Linear search and stops clock
            std::cout << "CPU time: " << microseconds <<
                " microseconds" << std::endl;
            // Prints CPU time to console
            std::ofstream dat_file(dat_out);
            dat_file << lin_matches;
            correct_choice++;
            // Output file containing # of matches is written
            break;
            // If user input is l, linear search function is called
        }
        case 'b': {
            // If user input is b, binary search function is called
            std::sort(pos_vec_vec.begin(), pos_vec_vec.end());
            Clock ct2;
            // Constructs clock object starting clock
            int bin_matches = BinSearch(pos_vec_vec, mag_vec);
            double microseconds = ct2.CurrentTime();
            // Computes binary search and stops clock
            std::cout << "CPU time: " << microseconds <<
                " microseconds" << std::endl;
            // Prints CPU time to console
            std::ofstream dat_file(dat_out);
            dat_file << bin_matches;
            // Output file containing # of matches is written
            correct_choice++;
            break;
        }
        default: {
            std::cerr << "Incorrect choice" << std::endl;
            // Any unintended input is thrown as an error
        }
      }
    }
}


