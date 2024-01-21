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

static char* get_file_extension(char* filename)
{
    char* dot = strrchr(filename, '.');

    if (!dot || dot == filename) {
        return "";
    } else {
        return dot + 1;
    }
}

int main(int argc, char *argv[])
{
    int ret = -1;
    huffman_io_t* huffman_io = NULL;
    huffman_tree_t* huffman_tree = NULL;
    huffman_codes_t huffman_codes = { 0 };
    char input_filepath[MAX_SIZE_FILENAME] = { 0 };
    char output_filepath[MAX_SIZE_FILENAME] = { 0 };
    char* input_extension = NULL;
    
    // A while loop is created to allow the user to navigate through the different
    // pages of the GUI. The while-loop will be exited if valid input/output files
    // are provided, or if the user decides to exit the program.
    while (true) {

        ret = gui__print_landing_page();
    
        if (ret == 0) {
            goto end;

        } else if (ret == 1) {
            ret = gui__print_compression_page(input_filepath, output_filepath);
            input_extension = get_file_extension(input_filepath);

            if (strcmp(input_extension, "txt") != 0) {
                ret = gui__print_error();
                if (ret == 1) { continue; }
                else { goto end; }
            }
            
            huffman_io = huffman_io__create(input_filepath, output_filepath, true);
            
            // If huffman_io couldn't be created, it allows the user to correct the
            // input/output filepaths provided.
            if (huffman_io == NULL) {
                ret = gui__print_error();
                if (ret == 1) { continue; }
                else { goto end; }
            } else {
                break;
            }
        
        } else if (ret == 2) {
            ret = gui__print_decompression_page(input_filepath, output_filepath);
            input_extension = get_file_extension(input_filepath);

            if (strcmp(input_extension, "bin") != 0) {
                ret = gui__print_error();
                if (ret == 1) { continue; }
                else { goto end; }
            }
        
            huffman_io = huffman_io__create(input_filepath, output_filepath, false);
            
            // If huffman_io couldn't be created, it allows the user to correct the
            // input/output filepaths provided.
            if (huffman_io == NULL) {
                ret = gui__print_error();
                if (ret == 1) { continue; }
                else { goto end; }
            } else {
                break;
            }
        }
    }

    // Once we exit the loop, we can proceed to compress/decompress the file

    if (huffman_io->compress == true) {
        // In this case, we create the Huffman Tree outside because we want to be able to
        // display it in the GUI.
        huffman_tree = huffman_tree__create();
        ret = huffman__compress(huffman_io, huffman_tree, &huffman_codes);
        gui__print_result_page_compressed(huffman_io, huffman_tree, &huffman_codes);

    } else {
        ret = huffman__decompress(huffman_io);
        gui__print_result_page_decompressed(huffman_io);
    }

    
end:

    // Clean-up

    if (huffman_io != NULL) {
        ret = huffman_io__destroy(huffman_io);
    }

    if (huffman_tree != NULL) {
        ret = huffman_tree__destroy(huffman_tree);
    }

    return ret;
}