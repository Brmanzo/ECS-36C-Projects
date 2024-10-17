#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include "bstream.h"

// Raj Garimella
// 919351073
// Bradley Manzo
// 916953788
// Programming Assignment #4
// Prof. Joel Porquet-Lupine
// May 20th, 2022


TEST(BStream, input) {
    std::string filename{ "test_bstream_input" };

    const unsigned char val[] = {
      0x58, 0x90, 0xab, 0x08,
      0x00, 0x4e, 0xdb, 0x40,
    };
    // Equivalent in binary is:
    // 0101100010010000101010110000100000000000010011101101101101000000
    // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

    // Write this to a file
    std::ofstream ofs(filename, std::ios::out |
        std::ios::trunc |
        std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(val), sizeof(val));
    ofs.close();

    // Read it back in binary format
    std::ifstream ifs(filename, std::ios::in |
        std::ios::binary);
    BinaryInputStream bis(ifs);

    // Make sure that we reading the binary in the correct order
    EXPECT_EQ(bis.GetBit(), 0);
    EXPECT_EQ(bis.GetBit(), 1);
    EXPECT_EQ(bis.GetChar(), 0x62);  // 01100010
    EXPECT_EQ(bis.GetChar(), 0x42);  // 01000010
    EXPECT_EQ(bis.GetBit(), 1);
    EXPECT_EQ(bis.GetInt(), 0x58400276);  // 01011000010000000000001001110110

    ifs.close();

    std::remove(filename.c_str());
}

TEST(BStream, output_char) {
    const unsigned char chars[] = {
      0x58, 0x90, 0xab, 0x08,
      0x00, 0x4e, 0xdb, 0x40,
    };
    // Equivalent in binary is:
    // 0101100010010000101010110000100000000000010011101101101101000000
    // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

    std::string filename{ "test_bstream_output_char" };

    std::ofstream ofs(filename, std::ios::out |
        std::ios::trunc |
        std::ios::binary);
    BinaryOutputStream bos(ofs);

    // Writes to a file using Binary Output Stream
    bos.PutChar(chars[0]);
    bos.PutChar(chars[1]);
    bos.PutChar(chars[2]);
    bos.PutChar(chars[3]);
    bos.PutChar(chars[4]);
    bos.PutChar(chars[5]);
    bos.PutChar(chars[6]);
    bos.PutChar(chars[7]);

    ofs.close();

    /*-----------------------------------------------------------------------*/
    unsigned char val[8];
    std::ifstream ifs(filename, std::ios::in |
        std::ios::binary);
    ifs.read(reinterpret_cast<char*>(val), sizeof(val));
    ifs.close();

    // reads each byte within the output file
    for (int i = 0; i < 8; i++) {
        ifs >> val[i];
    }

    ifs.close();

    // Make sure that each byte is what we wrote into the file
    EXPECT_EQ(val[0], 0x58);
    EXPECT_EQ(val[1], 0x90);
    EXPECT_EQ(val[2], 0xab);
    EXPECT_EQ(val[3], 0x08);
    EXPECT_EQ(val[4], 0x00);
    EXPECT_EQ(val[5], 0x4e);
    EXPECT_EQ(val[6], 0xdb);
    EXPECT_EQ(val[7], 0x40);

    std::remove(filename.c_str());
}

TEST(BStream, output_int) {
    const unsigned int ints[] = {
        0x5890ab08, 0x004edb40,
    };
    // Equivalent in binary is:
    // 0101100010010000101010110000100000000000010011101101101101000000
    // ^5  ^8  ^9  ^0  ^a  ^b  ^0  ^8  ^0  ^0  ^4  ^e  ^d  ^b  ^4  ^0

    std::string filename{ "test_bstream_output_int" };

    std::ofstream ofs(filename, std::ios::out |
        std::ios::trunc |
        std::ios::binary);
    BinaryOutputStream bos(ofs);

    // Writes to a file using Binary Output Stream
    bos.PutInt(ints[0]);
    bos.PutInt(ints[1]);

    ofs.close();

    /*-----------------------------------------------------------------------*/
    unsigned char val[8];
    std::ifstream ifs(filename, std::ios::in |
        std::ios::binary);
    ifs.read(reinterpret_cast<char*>(val), sizeof(val));
    ifs.close();


    // reads each byte within the output file
    for (int i = 0; i < 32; i++) {
        ifs >> val[i];
    }

    ifs.close();

    // Make sure that the file output word matches the integer input
    EXPECT_EQ(val[0], 0x58);
    EXPECT_EQ(val[1], 0x90);
    EXPECT_EQ(val[2], 0xab);
    EXPECT_EQ(val[3], 0x08);
    EXPECT_EQ(val[4], 0x00);
    EXPECT_EQ(val[5], 0x4e);
    EXPECT_EQ(val[6], 0xdb);
    EXPECT_EQ(val[7], 0x40);

    std::remove(filename.c_str());
}

TEST(BStream, output_interwoven) {
    std::string filename{ "test_bstream_output_interwoven" };

    std::ofstream ofs(filename, std::ios::out |
        std::ios::trunc |
        std::ios::binary);
    BinaryOutputStream bos(ofs);

    // Writes to a file using Binary Output Stream
    bos.PutBit(0);
    bos.PutBit(1);
    bos.PutChar('a');
    bos.PutBit(1);
    bos.PutBit(1);
    bos.PutBit(0);
    bos.PutInt(532);
    bos.PutBit(1);
    bos.PutChar('f');
    bos.PutChar('k');
    bos.PutBit(1);
    bos.PutBit(0);

    // Equivalent in binary is:
    // 0101100001110000000000000000000000010000101001011001100110101110
    // ^5  ^8  ^7  ^0  ^0  ^0  ^0  ^0  ^1  ^0  ^a  ^5  ^9  ^9  ^a  ^e

    ofs.close();

    /*-----------------------------------------------------------------------*/
    unsigned char val[8];
    std::ifstream ifs(filename, std::ios::in |
        std::ios::binary);
    ifs.read(reinterpret_cast<char*>(val), sizeof(val));
    ifs.close();


    // reads each byte within the output file
    for (int i = 0; i < 32; i++) {
        ifs >> val;
    }

    ifs.close();

    // Make sure that the file output word matches the integer input
    EXPECT_EQ(val[0], 0x58);
    EXPECT_EQ(val[1], 0x70);
    EXPECT_EQ(val[2], 0x00);
    EXPECT_EQ(val[3], 0x00);
    EXPECT_EQ(val[4], 0x10);
    EXPECT_EQ(val[5], 0xa5);
    EXPECT_EQ(val[6], 0x99);
    EXPECT_EQ(val[7], 0xae);

    std::remove(filename.c_str());
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
