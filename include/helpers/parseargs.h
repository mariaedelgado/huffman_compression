#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE_FILENAME   256

typedef struct {

    char input_file[MAX_SIZE_FILENAME];
    char output_file[MAX_SIZE_FILENAME];

    bool compress;

} parse_args_t;

/** \brief Parses the command line options and fills the parse_args_t struct with
 * the input/output information provided by the user.
 * \param argc as int. Number of arguments.
 * \param argv as char**. Array of arguments.
 * \param parse_args as parse_args_t*. Pointer to parse_args_t struct to be filled.
 * \return int. Error code (0 if OK).
*/
int parse_command_line_options(int argc, char** argv, parse_args_t* parse_args);