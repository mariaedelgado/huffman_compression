#include "../include/huffman.h"
#include "../include/symbols.h"

int hufmann__compress_file(FILE* fh_in, FILE* fh_out)
{
    int ret = -1;
    symbols_t symbols = { 0 };

    // First, the input file must be parsed and every character will be stored in
    // a symbol_t struct
    ret = symbols__get_from_input_file(&symbols, fh_in);
    symbols__print(&symbols);

    return ret;
}