#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// We are going to use the ASCII table to quickly access the Huffman codes of the characters. This is, we are going
// to create an array of 127 - 32 = 95 to represent each of the possible characters and access the table by their
// decimal index. Table of ASCII printable characters available in the source file.

#define N_ASCII_PRINTABLE_CHAR  95
#define N_MAX_SIGNIFICANT_BITS   8

struct huffman_codes {

    uint8_t n_significant_bits[N_ASCII_PRINTABLE_CHAR];         // Number of significant bits in the Huffman code
    char code[N_ASCII_PRINTABLE_CHAR][N_MAX_SIGNIFICANT_BITS];  // Array with the size of the maximum number of printable characters,
                                                                // where we are going to store the value of their Huffman code.
};

typedef struct huffman_codes huffman_codes_t;

int huffman_codes__encode_file(FILE* fh_in, FILE* fh_out, const huffman_codes_t* codes);