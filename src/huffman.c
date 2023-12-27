#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/huffman_io.h"
#include "../include/huffman.h"

int huffman__compress_file(FILE* fh_in, FILE* fh_out)
{
    int ret = -1;
    huffman_io_t huffman_io = {.fh_in = fh_in, .fh_out = fh_out};
    huffman_tree_t huffman_tree = { 0 };
    huffman_codes_t huffman_codes = { 0 };

    // First, the input file must be parsed and every character will be stored in
    // a symbol_t struct
    ret = huffman_io__read(&huffman_io, &huffman_tree);

    ret = huffman_tree__generate(&huffman_tree);

    ret = huffman_codes__generate(&huffman_codes, huffman_tree);

    // ret = huffman_codes__encode_file(fh_in, fh_out, &huffman_codes);
    ret = huffman_io__write(&huffman_io, &huffman_tree, &huffman_codes);

    // Destroy the structs that have allocated memory
    ret = huffman_tree__destroy(&huffman_tree);

    return ret;
}