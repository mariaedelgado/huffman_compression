#include <stdint.h>
#include <stdio.h>

#include "huffman_tree.h"

typedef struct {
    FILE* fh_in;
    FILE* fh_out;
} huffman_io_t;

/** \brief Reads input file to be encoded and generates an initial Huffman Tree that
 * contains only the leafs (the read characters with their respective frequencies).
* \param self as huffman_io_t*. Pointer to the huffman_io_t struct.
* \param huffman_tree as huffman_tree_t*. Output of the method that contains the initial
  Huffman tree containing the leafs.
* \return int. Error code (0 if OK).
*/
int huffman_io__read(huffman_io_t* self, huffman_tree_t* huffman_tree);
