#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "huffman_tree.h"

/* We are going to use the ASCII table to quickly access the Huffman codes of the characters. This is, we are going
   to create an array of 127 - 32 = 95 to represent each of the possible characters and access the table by their
   decimal index. Table of ASCII printable characters available in the source file.
   In order to take into account the EOF character, we will add an extra slot in the end. */

#define N_ASCII_PRINTABLE_CHAR  95
#define N_MAX_SIGNIFICANT_BITS  50

typedef struct {

    uint8_t n_significant_bits[N_ASCII_PRINTABLE_CHAR + 1];         // Number of significant bits in the Huffman code
    char code[N_ASCII_PRINTABLE_CHAR + 1][N_MAX_SIGNIFICANT_BITS];  // Array with the size of the maximum number of printable characters,
                                                                    // where we are going to store the value of their Huffman code.
} huffman_codes_t;

/** \brief Generate Huffman codes table from the tree of this Huffman process.
 * \param self as huffman_codes_t*. Pointer to the huffman_codes_t* table to be filled.
 * \param huffman_tree as huffman_tree_t*. Huffman Tree of the process.
 * \return int. Error code (0 if OK).
*/
int huffman_codes__generate(huffman_codes_t* self, huffman_tree_t* huffman_tree);

/** \brief Generated Huffman codes table by reading the header of the compressed file.
 * \param self as huffman_codes_t*. Pointer to the huffman_codes_t* table to be filled.
 * \param fh_in as FILE*. Pointer to the file handler to read.
 * \return int. Error code (0 if OK).
*/
int huffman_codes__generate_from_header(huffman_codes_t* self, FILE* fh_in);