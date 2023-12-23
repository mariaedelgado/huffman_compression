#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/huffman_tree.h"
#include "../include/huffman_codes.h"
#include "../include/huffman.h"

static int huffman__generate_code(const huffman_tree_t self, int current_index, char* codes, int level, huffman_codes_t* huffman_codes)
{
    int ret = -1;
    int pos = 0;

    const node_t* current_node = &self.node[current_index];
    const node_t* lchild_node = &self.node[current_node->lchild_index];
    const node_t* rchild_node = &self.node[current_node->rchild_index];

    if (is_leaf(lchild_node)) {
        codes[level - 1] = '0';
        pos = lchild_node->character - 32;
        huffman_codes->n_significant_bits[pos] = level;
        strcpy(huffman_codes->code[pos], codes);
        printf("Character: %c [%.*s]\n", lchild_node->character, level, huffman_codes->code[pos]);
    } else {
        codes[level - 1] = '0';
        huffman__generate_code(self, current_node->lchild_index, codes, level + 1, huffman_codes);
    }

    if (is_leaf(rchild_node)) {
        codes[level - 1] = '1';
        pos = rchild_node->character - 32;
        huffman_codes->n_significant_bits[pos] = level;
        strcpy(huffman_codes->code[pos], codes);
        printf("Character: %c [%.*s]\n", rchild_node->character, level, huffman_codes->code[pos]);
    } else {
        codes[level - 1] = '1';
        huffman__generate_code(self, current_node->rchild_index, codes, level + 1, huffman_codes);
    }

    return ret;
}

static int huffman_tree__generate_codes(const huffman_tree_t self, huffman_codes_t* huffman_codes)
{
    int ret = -1;
    int level = 1;  // Number of significant bits on each code
    char codes[MAX_TREE_HT] = { 0 };

    if (!huffman_codes)
        goto end;
    
    // Generate codes that are, for the moment, stored in the tree itself (TO BE ENHANCED)
    ret = huffman__generate_code(self, self.number_of_nodes - 1, codes, level, huffman_codes);

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

    ret = huffman_codes__encode_file(fh_in, fh_out, &huffman_codes);

    // Destroy the structs that have allocated memory
    ret = huffman_tree__destroy(&huffman_tree);
    ret = symbols__destroy(&symbols);

    return ret;
}