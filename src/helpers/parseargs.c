#include "../../include/helpers/parseargs.h"

int parse_command_line_options(int argc, char** argv, parse_args_t* parse_args)
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