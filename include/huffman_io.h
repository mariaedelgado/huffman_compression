#include <stdint.h>
#include <stdio.h>

#include "huffman_codes.h"

typedef struct {
      FILE* fh_in;
      FILE* fh_out;
      bool compress;
} huffman_io_t;

/** \brief Create huffman_io_t* instance.
 * \param input_file as char*. Input file to be compressed/decompressed.
 * \param output_file as char*. Output file where the decompressed/compressed data is
 * going to be stored in.
 * \param compress as bool. 'true' if input file is to be compressed, 'false' if
 * input file is to be decompressed.
 * \return huffman_io_t*.
*/
huffman_io_t* huffman_io__create(char* input_file, char* output_file, bool compress);

/** \brief Destroy huffman_io_t* instance.
 * \param self as huffman_io_t*. Pointer to the instance to be destroyed.
 * \return int. Error code (0 if OK).
*/
int huffman_io__destroy(huffman_io_t* self);

/** \brief Reads input file to be encoded and generates an initial Huffman Tree that
 * contains only the leafs (the read characters with their respective frequencies).
* \param self as huffman_io_t*. Pointer to the huffman_io_t struct.
* \param huffman_tree as huffman_tree_t*. Output of the method that contains the initial
  Huffman tree containing the leafs.
* \return int. Error code (0 if OK).
*/
int huffman_io__read(huffman_io_t* self, huffman_tree_t* huffman_tree);

/** \brief Write encoded output using the Huffman codes generated from the tree.
 * \param self as huffman_io_t*. Pointer to the huffman_io_t struct.
 * \param huffman_tree as huffman_tree_t*. Huffman tree to be added in the encoded
 * file so that we can decode the file later.
 * \param huffman_codes as huffman_codes_t*. Table containing the ASCII printable
 * characters and the equivalent Huffman code for each.
 * \return int. Error code (0 if OK).
*/
int huffman_io__write(huffman_io_t* self, huffman_tree_t* huffman_tree, huffman_codes_t* huffman_codes);