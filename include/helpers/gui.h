#include <stdio.h>

#include "../include/huffman/huffman_io.h"

/** \brief Prints welcome page and asks the user to decide the next step: Help page,
 * Compression page or Decompression page.
 * \return int. Error code (0 if OK).
**/ 
int gui__print_landing_page();

/** \brief Prints compression page. Asks the user to specify input and output filepaths.
 * \return int. Error code (0 if OK).
**/
int gui__print_compression_page(char* input_filepath, char* output_filepath);

/** \brief Prints decompression page. Asks the user to specify input file and output filepaths.
 * \return int. Error code (0 if OK).
**/
int gui__print_decompression_page(char* input_filepath, char* output_filepath);

/** \brief Prints result page with compression ratio. The user can also select to see a display
 * of the codes used as well as the Huffman Tree (for small ones).
 * \param huffman_io as huffman_io_t*. Pointer to huffman_io_t struct used to compute the compression
 * ratio.
 * \param huffman_tree as const huffman_tree_t*. Pointer to the Huffman Tree in case the user
 * needs it to be displayed.
 * \param huffman_codes as const huffman_codes_t*. Pointer to Huffman Codes in case the user needs it
 * to be displayed.
 * \return void.
**/
void gui__print_result_page_compressed(huffman_io_t* huffman_io, const huffman_tree_t* huffman_tree, const huffman_codes_t* huffman_codes);

/** \brief Prints result page with compression ratio.
 * \param huffman_io as huffman_io_t*. Pointer to huffman_io_t struct used to compute the compression
 * ratio.
 * \return void.
**/
void gui__print_result_page_decompressed(huffman_io_t* huffman_io);

/** \brief Prints error.
 * \return void.
**/
int gui__print_error();