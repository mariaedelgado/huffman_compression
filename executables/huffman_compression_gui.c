#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../include/helpers/gui.h"
#include "../include/helpers/parseargs.h"

static int huffman__compress(huffman_io_t* huffman_io, huffman_tree_t* huffman_tree, huffman_codes_t* huffman_codes)
{
    int ret = -1;
    
    // Read file and store the characters (and their frequency) as leafs of the Tree
    ret = huffman_io__read_file_to_compress(huffman_io, huffman_tree);
    if (ret != 0)
        goto end;

    // Generate Huffman Tree intermediate nodes
    ret = huffman_tree__generate(huffman_tree);
    if (ret != 0)
        goto end;
    
    // Generate the codes from the Huffman tree
    ret = huffman_codes__generate(huffman_codes, huffman_tree);
    if (ret != 0)
        goto end;
    
    // Re-write the file using the Huffman codes in the output file
    ret = huffman_io__write_compressed_file(huffman_io, huffman_tree, huffman_codes);
    if (ret != 0)
        goto end;

end:
    return ret;
}

static int huffman__decompress(huffman_io_t* huffman_io)
{
    int ret = -1;
    huffman_codes_t huffman_codes = { 0 };

    // Read file header to obtain the Huffman codes with which the file has been compressed
    ret = huffman_io__read_file_to_uncompress(huffman_io, &huffman_codes);
    if (ret != 0)
        goto end;
    
    // Use the codes to decode the compressed file and write it into the output file
    ret = huffman_io__write_uncompressed_file(huffman_io, &huffman_codes);
    if (ret != 0)
        goto end;

end:
    return ret;
}

int main(int argc, char *argv[])
{
    int ret = -1;
    huffman_io_t* huffman_io = NULL;
    huffman_tree_t* huffman_tree = NULL;
    huffman_codes_t huffman_codes = { 0 };
    char input_filepath[MAX_SIZE_FILENAME] = { 0 };
    char output_filepath[MAX_SIZE_FILENAME] = { 0 };
    
    ret = gui__print_landing_page();
    
    switch(ret) 
    {
        case -1: { gui__print_error(); goto end; }
        case 0:  { goto end; }
        case 1:
        {
            ret = gui__print_compression_page(input_filepath, output_filepath);
            huffman_io = huffman_io__create(input_filepath, output_filepath, true);
            break;
        }
        case 2:
        {
            ret = gui__print_decompression_page(input_filepath, output_filepath);
            huffman_io = huffman_io__create(input_filepath, output_filepath, false);
            break;
        }
    }


    if (huffman_io->compress == true) {
        huffman_tree = huffman_tree__create();
        ret = huffman__compress(huffman_io, huffman_tree, &huffman_codes);
        gui__print_result_page(huffman_io, huffman_tree, &huffman_codes);

    } else {
        ret = huffman__decompress(huffman_io);
    }

    
end:

    if (huffman_io != NULL) {
        ret = huffman_io__destroy(huffman_io);
    }

    if (huffman_tree != NULL) {
        ret = huffman_tree__destroy(huffman_tree);
    }

    return ret;
}