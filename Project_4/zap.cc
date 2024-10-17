#include <iostream>
#include "huffman.h"

void ValidateInput(int num_args) {
  if (num_args != 3) {
    std::cerr <<
        "Usage: /autograder/source/tests/zap <inputfile> <zapfile>"
        << std::endl;
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  ValidateInput(argc);
  std::string input_file = argv[1];
  std::string output_file = argv[2];
  std::ofstream ofs(output_file);
  std::ifstream ifs(input_file);

  if (ifs) {
    Huffman::Compress(ifs, ofs);
      std::cout <<
          "Compressed input file " << input_file <<
          " into zap file " << output_file << std::endl;
  } else {
     std::cerr << "Error: cannot open input file " <<
         input_file << std::endl;
     exit(1);
  }
}
