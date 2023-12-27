#include <stdbool.h>
#include <stdlib.h>

#include "../include/huffman_io.h"

int huffman_io__read(huffman_io_t* self, huffman_tree_t* huffman_tree)
{
    int ret = -1;

    if (self->fh_in == NULL)
    {
        goto end;
    }

    while (true)
    {
        char c = fgetc(self->fh_in);

        if (feof(self->fh_in))
        {
            ret = 0;
            break;
        }

        ret = huffman_tree__store_character(huffman_tree, c);
        if (ret != 0)
        {
            goto end;
        }
    }

    ret = huffman_tree__sort(huffman_tree);

end:
    return ret;
}

/* --------------------------------------------------------------------------------------------------- */

static int huffman_io__encode_node(huffman_io_t* self, huffman_tree_t* huffman_tree, int current_index)
{
    int ret = 0;

    if ((self == NULL) || (huffman_tree == NULL)) {
        ret = -1;
        goto end;
    }

    node_t* current_node = &huffman_tree->node[current_index];

    if (is_leaf(current_node)) {
        fprintf(self->fh_out, "1");
        fprintf(self->fh_out, "%c", current_node->character);

    } else {
        fprintf(self->fh_out, "0");
        ret = huffman_io__encode_node(self, huffman_tree, current_node->lchild_index);
        ret = huffman_io__encode_node(self, huffman_tree, current_node->rchild_index);
    }

end:
    return ret;
}

static int huffman_io__write_header(huffman_io_t* self, huffman_tree_t* huffman_tree)
{
    int ret = -1;

    if ((self->fh_out == NULL) || (huffman_tree == NULL)) {
        goto end;
    }

    // Explain Huffman tree encoding method !!!!!!!!!!!!!!!!!
    // Starting at root node
    ret = huffman_io__encode_node(self, huffman_tree, huffman_tree->number_of_nodes - 1);

end:
    return ret;
}

int huffman_io__write(huffman_io_t* self, huffman_tree_t* huffman_tree, huffman_codes_t* huffman_codes)
{
    int ret = -1;

    if ((self->fh_in == NULL) || (self->fh_out == NULL)) {
        goto end;
    }

    // First, write the header (containing the Huffman tree)
    ret = huffman_io__write_header(self, huffman_tree);

    // Rewind fh_in, since it has already been read
    rewind(self->fh_in);

    // Read the file character by character and print the respective Huffman code for the
    // given character in the output file. Write a space between the encoded header and the
    // encoded bytes so that we can distinguish between the Huffman Tree and the file itself.
    char c = ' ';
    int pos = 0;
    fprintf(self->fh_out, " ");

    while (true)
    {
        c = fgetc(self->fh_in);

        if (feof(self->fh_in)) {
            ret = 0;
            break;
        }

        // Since the character is at the same time the index in our huffman_code_t table (when substracted 32).
        pos = (int)c - 32;
        fprintf(self->fh_out, "%.*s", huffman_codes->n_significant_bits[pos], huffman_codes->code[pos]);
    }

end:
    return ret;
}