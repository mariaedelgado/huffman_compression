#include <stdint.h>
#include <stdio.h>

/** \brief Compress a file using Huffman compression.
* \param fh_in as FILE*. Input file containing the data to be compressed.
* \param fh_out as FILE*. Output file containing the compressed data.
* \return int. Error code (0 if OK).
*/
int huffman__compress_file(FILE* fh_in, FILE* fh_out);
