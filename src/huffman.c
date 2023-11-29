#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/huffman.h"
#include "../include/symbols.h"

struct node {
    char symbol;
    int frequency;

    struct node* lchild;
    struct node* rchild;
};

typedef struct node node_t;

typedef struct {

    size_t number_of_nodes;     // Count of the number of nodes (including leafs)
    node_t* node;               // Array containing all the nodes of the tree

} huffman_tree_t;

static int huffman_tree__insert_new_intermediate_node(huffman_tree_t* self, int left_index, int right_index)
{
    int itn_index = 0;
    int itn_frequency = 0;
    node_t* tmp = NULL;
    printf("Current number of nodes in tree: %lu\n", self->number_of_nodes);

    // Find position in array
    itn_frequency = self->node[left_index].frequency + self->node[right_index].frequency;
    for(int i = 0; i < self->number_of_nodes; i++)
    {
        if (itn_frequency < self->node[i].frequency)
        {
            itn_index = i;
            printf("New node's position in array is %u\n", i);
            break;
        }
    }

    // If itn_index is 0, it means that the frequency is greater or equal than the upmost node, so
    // we set it to the maximum index
    if (itn_index == 0)
    {
        itn_index = self->number_of_nodes;
    }

    // Resize array by one to fit the new node
    tmp = (node_t*)malloc(self->number_of_nodes*sizeof(node_t));
    memcpy(tmp, self->node, self->number_of_nodes*sizeof(node_t));

    self->number_of_nodes += 1;
    self->node = realloc(self->node, self->number_of_nodes*sizeof(node_t));

    // Generate intermediate node
    self->node[itn_index].frequency = self->node[left_index].frequency + self->node[right_index].frequency;
    self->node[itn_index].lchild = &self->node[left_index];
    self->node[itn_index].rchild = &self->node[right_index];

    // Move the next nodes one position up
    memcpy(&self->node[itn_index + 1], &tmp[itn_index], (self->number_of_nodes - itn_index - 1)*sizeof(node_t));

    free(tmp);

    for (int i = 0; i < self->number_of_nodes; i++)
    {
        if ((self->node[i].lchild != NULL) && (self->node[i].rchild != NULL)) {
            printf("[%u] Symbol: itn Frequency: %u\n", i, self->node[i].frequency);
        } else {
            printf("[%u] Symbol: %c Frequency: %u\n", i, self->node[i].symbol, self->node[i].frequency);
        }
    }
    return 0;
}

static int huffman_tree__generate(huffman_tree_t* self, const symbols_t* symbols)
{
    int ret = -1;
    int left_index = 0;
    int right_index = 0;
    int index_read = 0;

    if ((self == NULL) || (symbols == NULL))
        goto end;

    // First, create leaf nodes from the input symbols
    self->node = malloc(symbols->number_of_symbols*sizeof(node_t));
    self->number_of_nodes = symbols->number_of_symbols;

    for (int i = 0; i < self->number_of_nodes; i++)
    {
        self->node[i].symbol = symbols->symbol[i].value;
        self->node[i].frequency = symbols->symbol[i].frequency;
    }

    while (true)
    {
        // Find two min indexes from which we can create an intermediate node
        if (index_read >= self->number_of_nodes - 1)
            break;

        left_index = index_read;
        right_index = index_read + 1;
        index_read += 2;
        
        // Place new intermediate node in the array sorted by frequency (ascensing)
        ret = huffman_tree__insert_new_intermediate_node(self, left_index, right_index);
    }

end:
    return ret;
}

int huffman__compress_file(FILE* fh_in, FILE* fh_out)
{
    int ret = -1;
    symbols_t symbols = { 0 };
    huffman_tree_t huffman_tree = { 0 };

    // First, the input file must be parsed and every character will be stored in
    // a symbol_t struct
    ret = symbols__get_from_input_file(&symbols, fh_in);
    symbols__print(&symbols);

    huffman_tree__generate(&huffman_tree, &symbols);

    return ret;
}