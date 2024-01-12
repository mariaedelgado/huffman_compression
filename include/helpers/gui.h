#include <stdio.h>

#include "../include/huffman/huffman_io.h"

// Prints welcome and makes you decide between help, compression or decompression
int gui__print_landing_page();

// Prints compression page. Asks to specify input file and output filepath.
int gui__print_compression_page(char* input_filepath, char* output_filepath);

// Prints decompression page. Asks to specify input file and output filepath.
int gui__print_decompression_page(char* input_filepath, char* output_filepath);

// Prints page with compression ratio and output file. You can select the option to check the huffman tree
void gui__print_result_page(huffman_io_t* huffman_io, const huffman_tree_t* huffman_tree, const huffman_codes_t* huffman_codes);

void gui__print_error();