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
