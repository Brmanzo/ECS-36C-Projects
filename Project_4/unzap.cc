#include <iostream>
#include "huffman.h"

void ValidateInput(int num_args) {
  if (num_args != 3) {
    std::cerr <<
        "Usage: /autograder/source/tests/unzap <zapfile> <outputfile>"
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
    Huffman::Decompress(ifs, ofs);
      std::cout << "Decompressed input zap file "
          << input_file << " into file "
          << output_file << std::endl;
  } else {
     std::cerr << "Error: cannot open zap file " << input_file << std::endl;
     exit(1);
  }
}
