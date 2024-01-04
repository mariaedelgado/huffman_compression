#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE_FILENAME   256

typedef struct {

    char input_file[MAX_SIZE_FILENAME];
    char output_file[MAX_SIZE_FILENAME];

    bool compress;

} parse_args_t;

int parse_command_line_options(int argc, char** argv, parse_args_t* parse_args);