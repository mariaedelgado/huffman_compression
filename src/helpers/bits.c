#include "../include/helpers/bits.h"

uint8_t read_bits(FILE* file, uint8_t n_bits)
{
    uint8_t bits = 0;
    static uint8_t buffer = 0;      // Buffer to store the bytes read from the file
    static uint8_t buffer_pos = 8;  // Position of the next bit to read in the buffer
    
    while (n_bits > 0)
    {
        uint8_t mask = 0;
        uint8_t tmp_bits = 0;
        uint8_t n_bits_to_read_from_buffer = 0;

        if (buffer_pos == 8)
        {
            // In the case all the bits from the buffer have been read, read one more byte from the file
            fread(&buffer, sizeof(uint8_t), 1, file);
            buffer_pos = 0;
        }

        // Calculate the number of bits to be read from the current byte. If the n_bits fits in the current
        // buffer, we can read all of them. If not, we only read the ones that fit (8 - buffer_pos).
        n_bits_to_read_from_buffer = (n_bits < (8 - buffer_pos)) ? n_bits : (8 - buffer_pos);

        // Now, we read the necessary bits from the buffer. First, we shift to the right 8 - (n_bits + buffer_pos).
        // This way, we have all the bits we want to read to the right. Then, we apply a mask corresponding to the
        // number of bits that are to be read (n_bits_to_read_from_buffer).
        tmp_bits = buffer >> (8 - n_bits_to_read_from_buffer - buffer_pos);
        mask = (1 << n_bits_to_read_from_buffer) - 1;
        tmp_bits = tmp_bits & mask;

        // Now, add these bits into the result
        bits = (bits << n_bits_to_read_from_buffer) | tmp_bits;

        // Increase buffer_pos by the number of bits that have been read, and decrease n_bits too.
        buffer_pos += n_bits_to_read_from_buffer;
        n_bits -= n_bits_to_read_from_buffer;   
    }

    return bits;
}

int write_bits(FILE* file, uint8_t bits, uint8_t n_bits, uint8_t* buffer, uint8_t* buffer_pos)
{
    int ret = 0;
    // In this case, buffer and buffer_pos are input and not set as static variables because at
    // some point we need buffer and buffer_pos outside of this method.

    if (file == NULL) {
        ret = -1;
        goto end;
    }

    if (n_bits > 8) {
        ret = 1;
        goto end;
    }

    while (n_bits > 0)
    {
        uint8_t mask = 0;
        uint8_t bits_to_write = 0;
        uint8_t n_bits_to_write_from_current_byte = 0;

        // Determine how many bits to write in the current buffer
        n_bits_to_write_from_current_byte = (n_bits < (8 - *buffer_pos)) ? n_bits : (8 - *buffer_pos);

        // Obtain the bits to write using the proper mask
        bits_to_write = (bits >> (n_bits - n_bits_to_write_from_current_byte));
        mask = (1 << n_bits_to_write_from_current_byte) - 1;
        bits_to_write = bits_to_write & mask;

        // Combine with the existing bits in buffer
        *buffer = (*buffer << n_bits_to_write_from_current_byte) | bits_to_write;

        *buffer_pos += n_bits_to_write_from_current_byte;
        n_bits -= n_bits_to_write_from_current_byte;

        // In buffer is full, write the byte to the output file. Then set the parameters to
        // zero and keep reading.
        if (*buffer_pos == 8) 
        {
            // If buffer is complete, write it to the output file
            fwrite(buffer, sizeof(uint8_t), 1, file);
            *buffer = 0;
            *buffer_pos = 0;
        }
    }

end:
    return ret;
}