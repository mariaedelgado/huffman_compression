#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "../include/helpers/parseargs.h"
#include "../include/huffman/huffman_io.h"

static void print_compression_ratio(huffman_io_t* huffman_io)
{
    long input_file_size = 0;
    long output_file_size = 0;
    float compression_ratio = 0.0;

    // Compute input file size
    rewind(huffman_io->fh_in);
    fseek(huffman_io->fh_in, 0L, SEEK_END);
    input_file_size = ftell(huffman_io->fh_in);

    // Compute output file size
    rewind(huffman_io->fh_out);
    fseek(huffman_io->fh_out, 0L, SEEK_END);
    output_file_size = ftell(huffman_io->fh_out);

    // Print ratio
    compression_ratio = (float)output_file_size/(float)input_file_size;
    printf("\nCompression ratio = %f\n", compression_ratio);

    return;
}

static int huffman__compress(huffman_io_t* huffman_io)
{
    int ret = -1;
    huffman_tree_t huffman_tree = { 0 };
    huffman_codes_t huffman_codes = { 0 };
    
    // Read file and store the characters (and their frequency) as leafs of the Tree
    ret = huffman_io__read_file_to_compress(huffman_io, &huffman_tree);
    if (ret != 0)
        goto end;

    // Generate Huffman Tree intermediate nodes
    ret = huffman_tree__generate(&huffman_tree);
    if (ret != 0)
        goto end;
    
    // Generate the codes from the Huffman tree
    ret = huffman_codes__generate(&huffman_codes, &huffman_tree);
    if (ret != 0)
        goto end;
    
    // Re-write the file using the Huffman codes in the output file
    ret = huffman_io__write_compressed_file(huffman_io, &huffman_tree, &huffman_codes);
    if (ret != 0)
        goto end;

    // Print compression ratio
    print_compression_ratio(huffman_io);

    // Clean
    free(huffman_tree.node);

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
    parse_args_t parse_args = { 0 };
    
    // Parse the input command to obtain the type of process (compression/decompression), the input
    // file to be compressed and the output file where we are going to store the compressed data.

    ret = parse_command_line_options(argc, argv, &parse_args);
    if (ret != 0)
        goto end;

    huffman_io_t* huffman_io = huffman_io__create(parse_args.input_file,
                                                  parse_args.output_file,
                                                  parse_args.compress);

    if (huffman_io == NULL) {
        printf("[Error] huffman_io_t object could not be created. Please check if you provided a correct filepath.\n");
        ret = -1;
        goto end;
    }

    // We can proceed to compress/decompress the file
    if (huffman_io->compress == true) {
        printf("Start compression...\n     - Input file: %s\n     - Output file: %s\n",
                parse_args.input_file, parse_args.output_file);
        ret = huffman__compress(huffman_io);

    } else {
        printf("Start decompression...\n     - Input file: %s\n     - Output file: %s\n",
                parse_args.input_file, parse_args.output_file);
        ret = huffman__decompress(huffman_io);
    }
    
end:

    // Clean-up

    if (huffman_io != NULL) {
        ret = huffman_io__destroy(huffman_io);
    }

    return ret;
}