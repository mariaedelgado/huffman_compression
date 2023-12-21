#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/huffman.h"
#include "../include/huffman_codes.h"
#include "../include/symbols.h"

#define MAX_TREE_HT 50  // try to enhance

struct node {
    char character;
    int frequency;

    struct node* lchild;
    struct node* rchild;
};

typedef struct node node_t;

typedef struct {

    size_t number_of_nodes;     // Count of the number of nodes (including leafs)
    node_t* node;               // Array containing all the nodes of the tree

} huffman_tree_t;

static bool is_leaf(node_t* node)
{
    return ((!node->lchild) && (!node->rchild)) ? true : false;
}

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
        if (is_leaf(&self->node[i]) == true) {
            printf("[%u] character: %c Frequency: %u\n", i, self->node[i].character, self->node[i].frequency);
        } else {
            printf("[%u] character: %c%c Frequency: %u\n", i, self->node[i].lchild->character, self->node[i].rchild->character, self->node[i].frequency);
        };
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
        self->node[i].character = symbols->symbol[i].value;
        self->node[i].frequency = symbols->symbol[i].frequency;
        self->node[i].lchild = NULL;
        self->node[i].rchild = NULL;
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

static int huffman_tree__destroy(huffman_tree_t* self)
{
    int ret = -1;

    if (self == NULL)
        goto end;

    self->number_of_nodes = 0;
    free(self->node);
    ret = 0;

end:
    return ret;
}

static uint8_t assemble_code(int* codes, int level)
{
    uint8_t code = codes[0];

    for (int i = 1; i <= level; i++)
    {
        code = code << 1;
        code = code | codes[i];
        // code |= (code << 1) | codes[i];
    }

    return code;
}

static int huffman__generate_code(node_t* current_node, int* codes, int level, huffman_codes_t* huffman_codes)
{
    int ret = -1;
    int pos = 0;

    if (is_leaf(current_node->lchild)) {
        codes[level] = 0;
        pos = current_node->lchild->character - 32;
        huffman_codes->code[pos] = assemble_code(codes, level);
        huffman_codes->n_significant_bits[pos] = level;
        printf("Character: %c [%.*u]\n", current_node->lchild->character, level, huffman_codes->code[pos]);
    } else {
        codes[level] = 0;
        huffman__generate_code(current_node->lchild, codes, level + 1, huffman_codes);
    }

    if (is_leaf(current_node->rchild)) {
        codes[level] = 1;
        pos = current_node->rchild->character - 32;
        huffman_codes->code[pos] = assemble_code(codes, level);
        huffman_codes->n_significant_bits[pos] = level;
        printf("Character: %c [%.*u]\n", current_node->rchild->character, level, huffman_codes->code[pos]);
    } else {
        codes[level] = 1;
        huffman__generate_code(current_node->rchild, codes, level + 1, huffman_codes);
    }

    return ret;
}

static int huffman_tree__generate_codes(const huffman_tree_t self, huffman_codes_t* huffman_codes)
{
    int ret = -1;
    int level = 1;  // Number of significant bits on each code
    int codes[MAX_TREE_HT] = { 0 };
    node_t top_node = { 0 };

    if (!huffman_codes)
        goto end;
    
    // Generate codes that are, for the moment, stored in the tree itself (TO BE ENHANCED)
    top_node = self.node[self.number_of_nodes - 1];
    ret = huffman__generate_code(&top_node, codes, level, huffman_codes);

end:
    return ret;
}

int huffman__compress_file(FILE* fh_in, FILE* fh_out)
{
    int ret = -1;
    symbols_t symbols = { 0 };
    huffman_tree_t huffman_tree = { 0 };
    huffman_codes_t huffman_codes = { 0 };

    // First, the input file must be parsed and every character will be stored in
    // a symbol_t struct
    ret = symbols__get_from_input_file(&symbols, fh_in);
    symbols__print(&symbols);

    ret = huffman_tree__generate(&huffman_tree, &symbols);

    ret = huffman_tree__generate_codes(huffman_tree, &huffman_codes);

    // Destroy the structs that have allocated memory
    ret = huffman_tree__destroy(&huffman_tree);
    ret = symbols__destroy(&symbols);

    return ret;
}