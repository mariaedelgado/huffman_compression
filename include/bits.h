#include <stdio.h>
#include <stdint.h>

/** \brief Reads the next specificied number of bits from the given input file.
 * \param file as FILE*. Pointer to the file handler to read.
 * \param n_bits as uint8_t. Number of bits to be read (note that the maximum is
 * 8 bits - 1 byte).
 * \return uint8_t. Resulting bits from the operation.
*/
uint8_t read_bits(FILE* file, uint8_t n_bits);

/** \brief Writes in the file in binary representation. This means, the given bits
 * will only be written if the buffer is full (the 8 bits are complete).
 * \param file as FILE*. Pointer to the file handler to write on.
 * \param bits as uint8_t. Bit(s) to be written.
 * \param n_bits as uint8_t. Number of bits to be written.
 * \param buffer as uint8_t*. Pointer to the buffer where the bits to be written
 * will be stored until it is full and the infomation is written.
 * \param buffer_pos as uint8_t*. Pointer to the position in the buffer to be filled.
 * \return int. Error code (0 if OK).
*/
int write_bits(FILE* file, uint8_t bits, uint8_t n_bits, uint8_t* buffer, uint8_t* buffer_pos);