#include <stdbool.h>
#include <stdlib.h>

#include "../include/bits.h"
#include "../include/huffman_io.h"

huffman_io_t* huffman_io__create(char* input_file, char* output_file, bool compress)
{
    huffman_io_t* self = malloc(sizeof(huffman_io_t));

    // Open both input and output files. If we want to compress, our input is written
    // with regular ASCII, but our output file shall be in binary format. On the other
    // hand, if we want to decompress, our input is in binary but we will want to write
    // the output in ASCII format.

    self->compress = compress;
    if (self->compress == true) {
        self->fh_in = fopen(input_file, "r");
        self->fh_out = fopen(output_file, "wb+");

    } else {
        self->fh_in = fopen(input_file, "rb");
        self->fh_out = fopen(output_file, "w+");
    }

    if ((self->fh_in == NULL) || (self->fh_in == NULL))
    {
        self = NULL;
    }

    return self;
}

int huffman_io__destroy(huffman_io_t* self)
{
    int ret = 0;

    if (self == NULL) {
        ret = 1;
    } else {
        if (self->fh_in) fclose(self->fh_in);
        if (self->fh_out) fclose(self->fh_out);
        free(self);
    }

    return ret;
}

/* ----------------------------------------------------------------------------------- */

int huffman_io__read_file_to_compress(huffman_io_t* self, huffman_tree_t* huffman_tree)
{
    int ret = -1;

    if (self->fh_in == NULL)
    {
        goto end;
    }

    while (true)
    {
        int c = fgetc(self->fh_in);

        if (feof(self->fh_in))
        {
            ret = 0;
            break;
        }

        ret = huffman_tree__store_character(huffman_tree, (char)c);
        if (ret != 0)
        {
            goto end;
        }
    }

    ret = huffman_tree__sort(huffman_tree);

end:
    return ret;
}

int huffman_io__read_file_to_uncompress(huffman_io_t* self, huffman_codes_t* huffman_codes)
{
    return huffman_codes__generate_from_header(huffman_codes, self->fh_in);
}

/* --------------------------------------------------------------------------------------------------- */

static int huffman_io__encode_node(huffman_io_t* self, huffman_tree_t* huffman_tree, int current_index,
                                   char* header, uint8_t* header_pos)
{
    int ret = 0;

    if ((self == NULL) || (huffman_tree == NULL)) {
        ret = -1;
        goto end;
    }

    node_t* current_node = &huffman_tree->node[current_index];

    if (is_leaf(current_node)) {
        header[*header_pos] = '1';
        header[*header_pos + 1] = current_node->character;
        *header_pos += 2;

    } else {
        header[*header_pos] = '0';
        *header_pos += 1;

        ret = huffman_io__encode_node(self, huffman_tree, current_node->lchild_index, header, header_pos);
        ret = huffman_io__encode_node(self, huffman_tree, current_node->rchild_index, header, header_pos);
    }

end:
    return ret;
}

static int huffman_io__print_header(huffman_io_t* self, char* header, uint8_t header_pos,
                                    uint8_t* buffer, uint8_t* n_bits_in_buffer)
{
    int ret = 0;

    if ((self == NULL) || (header == NULL))
        goto end;

    if ((buffer == NULL) || (n_bits_in_buffer == NULL))
        goto end;

    for (int i = 0; i < header_pos; i++)
    {
        char bit = header[i];

        if ((bit == '0') || (bit == '1')) {
            write_bits(self->fh_out, bit, 1, buffer, n_bits_in_buffer);

        } else {
            write_bits(self->fh_out, bit, 8, buffer, n_bits_in_buffer);
        }
    }

    // If remaining bits in buffer, they will be written in the next step (we are
    // using the same buffer for header and body)

end:
    return ret;
}

static int huffman_io__write_header(huffman_io_t* self, huffman_tree_t* huffman_tree,
                                    uint8_t* buffer, uint8_t* n_bits_in_buffer)
{
    int ret = -1;
    char header[256] = { 0 };
    uint8_t header_pos = 0;

    if ((self->fh_out == NULL) || (huffman_tree == NULL)) {
        goto end;
    }

    // Explain Huffman tree encoding method !!!!!!!!!!!!!!!!!
    // Starting at root node
    ret = huffman_io__encode_node(self, huffman_tree, huffman_tree->number_of_nodes - 1, header, &header_pos);
    if (ret != 0)
        goto end;

    ret = huffman_io__print_header(self, header, header_pos, buffer, n_bits_in_buffer);

end:
    return ret;
}

int huffman_io__write_compressed_file(huffman_io_t* self, huffman_tree_t* huffman_tree, huffman_codes_t* huffman_codes)
{
    int ret = -1;
    uint8_t buffer = 0;
    uint8_t n_bits_in_buffer = 0;

    if ((self->fh_in == NULL) || (self->fh_out == NULL)) {
        goto end;
    }

    // First, write the header (containing the Huffman tree)
    ret = huffman_io__write_header(self, huffman_tree, &buffer, &n_bits_in_buffer);

    // Rewind fh_in, since it has already been read
    rewind(self->fh_in);

    // Read the file character by character and print the respective Huffman code for the
    // given character in the output file. 
    char c = ' ';
    int pos = 0;

    while (true)
    {
        // Read next character from the input file
        c = fgetc(self->fh_in);

        if (feof(self->fh_in)) {
            ret = 0;
            break;
        }

        // Since the character is at the same time the index in our huffman_code_t table (when substracted 32).
        pos = (int)c - 32;
        
        // Fill bits with the given code
        for (int i = 0; i < huffman_codes->n_significant_bits[pos]; i++)
        {
            // We can cast one of the chars into a uint8_t because we know it is either a 0 or a 1
            char bit = huffman_codes->code[pos][i];
            write_bits(self->fh_out, bit, 1, &buffer, &n_bits_in_buffer);
        }


    }

end:
    return ret;
}

int huffman_io__write_uncompressed_file(huffman_io_t* self, huffman_codes_t* huffman_codes)
{
    int ret = 0;
    char code[8] = { 0 };
    int counter = 0;
    bool found = false;

    if ((self == NULL) || (huffman_codes == NULL)) {
        ret = -1;
        goto end;
    }

    while (true) {

        // Read bit by bit. To convert from uint8_t to char, we add + '0'.
        code[counter] = read_bits(self->fh_in, 1) + '0';

        if (feof(self->fh_in)) {
            ret = 0;
            break;
        }

        // Try to map current code to an entry of the table. If not possible, continue reading
        for (int i = 0; i < N_ASCII_PRINTABLE_CHAR; i++)
        {
            if (strcmp(huffman_codes->code[i], code) == 0) {
                fprintf(self->fh_out, "%c", i + 32);
                found = true;
                break;
            }
        }

        if (found == false) {
            counter++;
        } else {
            found = false;
            memset(code, 0, counter + 1);
            counter = 0;
        }

    }

end:
    return ret;
}