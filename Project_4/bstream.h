#ifndef BSTREAM_H_
#define BSTREAM_H_

#include <cstddef>
#include <fstream>
#include <iostream>

class BinaryInputStream {
 public:
  explicit BinaryInputStream(std::ifstream &ifs);

  bool GetBit();
  char GetChar();
  int GetInt();

 private:
  std::ifstream &ifs;
  char buffer = 0;
  size_t avail = 0;

  // Helpers
  void RefillBuffer();
};

BinaryInputStream::BinaryInputStream(std::ifstream &ifs) : ifs(ifs) { }

void BinaryInputStream::RefillBuffer() {
  // Read the next byte from the input stream
  ifs.get(buffer);
  if (ifs.gcount() != 1)
    throw std::underflow_error("No more characters to read");
  avail = 8;
}

bool BinaryInputStream::GetBit() {
  bool bit;

  if (!avail)
    RefillBuffer();

  avail--;
  bit = ((buffer >> avail) & 1) == 1;

#if 0  // Switch to 1 for debug purposes
  if (bit)
    std::cout << '1' << std::endl;
  else
    std::cout << '0' << std::endl;
#endif

  return bit;
}

char BinaryInputStream::GetChar() {
    char character;
    // For 8 bits (1 byte) within ifstream object
    for (int i = 0; i < 8; ++i) {
        // New bit is inserted into the LSB of character
        // and is shifted left to recieve next bit
        character = (character << 1) | GetBit();
    }
    return character;
}

int BinaryInputStream::GetInt() {
    int number = 0;
    // For 32 bits (4 bytes) within ifstream object
    for (int i = 0; i < 32; ++i) {
        // New bit is inserted into the LSB of number
        // and is shifted left to recieve next bit
        number = (number << 1) | GetBit();
    }
    return number;
}

class BinaryOutputStream {
 public:
  explicit BinaryOutputStream(std::ofstream &ofs);
  ~BinaryOutputStream();

  void Close();

  void PutBit(bool bit);
  void PutChar(char byte);
  void PutInt(int word);

 private:
  std::ofstream &ofs;
  char buffer = 0;
  size_t count = 0;

  // Helpers
  void FlushBuffer();
};

BinaryOutputStream::BinaryOutputStream(std::ofstream &ofs) : ofs(ofs) { }

BinaryOutputStream::~BinaryOutputStream() {
  Close();
}

void BinaryOutputStream::Close() {
  FlushBuffer();
}

void BinaryOutputStream::FlushBuffer() {
  // Nothing to flush
  if (!count)
    return;

  // If buffer isn't complete, pad with 0s before writing
  if (count > 0)
    buffer <<= (8 - count);

  // Write to output stream
  ofs.put(buffer);

  // Reset buffer
  buffer = 0;
  count = 0;
}

void BinaryOutputStream::PutBit(bool bit) {
  // Make some space and add bit to buffer
  buffer <<= 1;
  if (bit)
    buffer |= 1;

  // If buffer is full, write it
  if (++count == 8)
    FlushBuffer();
}

void BinaryOutputStream::PutChar(char byte) {
    bool bit = 0;
    int shift_right_amount = 7;

    // For all 8 bits within byte
    for (int i = 0; i < 8; i++) {
        int temp_byte = byte;
        // Shift right the appropriate distance to retrieve
        // the proper bit and put it to the file out stream
        bit = ((temp_byte >> shift_right_amount) & 1);
        PutBit(bit);
        shift_right_amount--;
    }
// PutBit to ofstream
}

void BinaryOutputStream::PutInt(int word) {
    bool bit = 0;
    int shift_right_amount = 31;

    // For all 32 bits within word
    for (int i = 0; i < 32; i++) {
        int temp_word = word;
        // Shift right the appropriate distance to retrieve
        // the proper bit and put it to the file out stream
        bit = ((temp_word >> shift_right_amount) & 1);
        PutBit(bit);
        shift_right_amount--;
    }
}

#endif  // BSTREAM_H_
