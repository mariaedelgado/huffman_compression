#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_TREE_HT 50  // try to enhance

typedef struct {
    char character;
    int frequency;

    int lchild_index;
    int rchild_index;
} node_t;

bool is_leaf(const node_t* node);

typedef struct {

    size_t number_of_nodes;     // Count of the number of nodes (including leafs)
    node_t* node;               // Array containing all the nodes of the tree

} huffman_tree_t;

int huffman_tree__store_character(huffman_tree_t* self, char character);

int huffman_tree__sort(huffman_tree_t* self);

int huffman_tree__generate(huffman_tree_t* self);

int huffman_tree__destroy(huffman_tree_t* self);