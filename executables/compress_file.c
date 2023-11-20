#include <stdio.h>
#include <string.h>

#include "../include/huffman.h"

#define MAX_SIZE_FILENAME   256

static int parse_command_line_options(int argc, char** argv, char* input_file, char* output_file)
{
    int ret = -1;

    if ((argc == 2) && (strcmp(argv[1], "--help") == 0))
    {
        printf("Usage: compress_file [file_in] [file_out] \n");
        ret = 0;

    } else if (argc == 3) {

        strcpy(input_file, argv[1]);
        strcpy(output_file, argv[2]);
        ret = 1;

    } else  {

        printf("ERROR");
        ret = -1;
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int ret = -1;
    char input_file[MAX_SIZE_FILENAME];
    char output_file[MAX_SIZE_FILENAME];
    
    // Parse the input command to obtain the input file to be compressed and
    // the output file where we are going to store the compressed data.
    ret = parse_command_line_options(argc, argv, input_file, output_file);
    if (ret != 1) 
    {
        goto end;
    }

    // Open both input and output files
    FILE* fh_in = fopen(input_file, "r");
    FILE* fh_out = fopen(output_file, "w+");

    if ((fh_in == NULL) || (fh_out == NULL))
    {
        ret = -1;
        goto end;
    }

    // Call method to compress file
    ret = hufmann__compress_file(fh_in, fh_out);

end:
    if (fh_in)
        fclose(fh_in);

    if (fh_out)
        fclose(fh_out);

    return ret;
}