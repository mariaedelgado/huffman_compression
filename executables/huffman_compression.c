#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../include/huffman/huffman_io.h"

#define MAX_SIZE_FILENAME   256

struct parse_args {

    char input_file[MAX_SIZE_FILENAME];
    char output_file[MAX_SIZE_FILENAME];

    bool compress;

} parse_args;

typedef struct parse_args parse_args_t;

static int parse_command_line_options(int argc, char** argv, parse_args_t* parse_args)
{
    int ret = -1;

    // Help
    if ((argc == 2) && (strcmp(argv[1], "--help") == 0))
    {
        printf("Usage:  huffman_compression -c [-i <file_in>] [-o <file_out>] \n"
               "        huffman_compression -d [-i <file_in>] [-o <file_out>]\n");
        ret = 1;

    } else if (argc == 6) {

        // First argument specifies if we want to compress or decompress input files.
        if ((strcmp(argv[1], "-c") == 0))
        {
            parse_args->compress = true;

        } else if ((strcmp(argv[1], "-d") == 0)) {
            parse_args->compress = false;

        } else {
            printf("ERROR: it is necessary to specify if you want to compress (-c) or decompress (-d) the input file.\n"
                   "Type 'huffman_compression --help' for usage.\n");
            ret = -1;
            goto end;
        }

        // Then, we get the input and output files and store them in the output struct.
        if ((strcmp(argv[2], "-i") == 0) && (strcmp(argv[4], "-o") == 0)) {

            strcpy(parse_args->input_file, argv[3]);
            strcpy(parse_args->output_file, argv[5]);
            ret = 0;

        } else if ((strcmp(argv[2], "-o") == 0) && (strcmp(argv[4], "-i") == 0)) {

            strcpy(parse_args->output_file, argv[3]);
            strcpy(parse_args->input_file, argv[5]);
            ret = 0;
            
        } else {

            printf("ERROR: input and output files have to be specified.\n"
                   "Type 'huffman_compression --help' for usage.\n");
            ret = -1;
            goto end;
        }

    } else  {

        printf("ERROR: wrong number of arguments.\n"
                "Type 'huffman_compression --help' for usage.\n");
        ret = -1;
    }

end:
    return ret;
}

/* ----------------------------------------------------------------------------------- */

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
    ret = huffman_codes__generate(&huffman_codes, huffman_tree);
    if (ret != 0)
        goto end;
    
    // Re-write the file using the Huffman codes in the output file
    ret = huffman_io__write_compressed_file(huffman_io, &huffman_tree, &huffman_codes);
    if (ret != 0)
        goto end;

    ret = huffman_tree__destroy(&huffman_tree);

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

static void print_compression_ratio(huffman_io_t* huffman_io)
{
    int input_file_size = 0;
    int output_file_size = 0;
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
    printf("Compression ratio = %d/%d = %f\n", output_file_size, input_file_size, compression_ratio);

    return;
}

int main(int argc, char *argv[])
{
    int ret = -1;
    parse_args_t parse_args = { 0 };
    
    // Parse the input command to obtain the input file to be compressed and
    // the output file where we are going to store the compressed data.
    ret = parse_command_line_options(argc, argv, &parse_args);
    if (ret != 0)
        goto end;

    huffman_io_t* huffman_io = huffman_io__create(parse_args.input_file,
                                                  parse_args.output_file,
                                                  parse_args.compress);

    if (huffman_io->compress == true) {
        ret = huffman__compress(huffman_io);
        print_compression_ratio(huffman_io);

    } else {
        ret = huffman__decompress(huffman_io);
    }

    
end:

    if (huffman_io != NULL) {
        ret = huffman_io__destroy(huffman_io);
    }

    return ret;
}