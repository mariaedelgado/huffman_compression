#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char character;
    int frequency;

    int lchild_index;
    int rchild_index;
} node_t;

/** \brief Asserts if input node is a leaf.
 * \param node as const node_t*. Pointer to the node that we want to assert.
 * \return bool. 'true' if leaf node, 'false' if intermediate node.
 * 
*/
bool is_leaf(const node_t* node);

/* ---------------------------------------------------------------------------------- */

typedef struct {

    size_t number_of_nodes;     // Count of the number of nodes (including leafs)
    node_t* node;               // Array containing all the nodes of the tree

} huffman_tree_t;

/** \brief Stores character in the Huffman Tree (as a leaf node, since this method)
 * is to be called when reading an input file to encode.
 * \param self as huffman_tree_t*. Pointer to the Huffman Tree to be filled.
 * \param character as char. Character to be added in the Huffman Tree as a leaf.
 * If the character is already present, its frequency will go up by one unit.
 * \return int. Error code (0 if OK). 
*/
int huffman_tree__store_character(huffman_tree_t* self, char character);

/** \brief Stores an EOF character with frequency 1. This code will be used to denote
 * the end of the encoded bits.
 * \param self as huffman_tree_t*. Pointer to the Huffman Tree.
 * \return int. Error code (0 if OK).
*/
int huffman_tree__add_eof(huffman_tree_t* self);

/** Sorts the Huffman Tree once all the file has been read and all the characters
 * have been added, in ascending order (by frequency).
 * \param self as huffman_tree_t*. Pointer to the Huffman Tree to be sorted.
 * \return int. Error code (0 if OK).
*/
int huffman_tree__sort(huffman_tree_t* self);

/** \brief Generates the Huffman Tree from the leaf nodes, creating the necessary
 * intermediate nodes to complete it.
 * \param self as huffman_tree_t*. Pointer to the Huffman Tree.
 * \return int. Error code (0 if OK).
*/
int huffman_tree__generate(huffman_tree_t* self);

/** \brief Destroyer of the Huffman Tree instance.
 * \param self as huffman_tree_t*. Pointer to the Huffman Tree.
 * \return int. Error code (0 if OK).
*/
int huffman_tree__destroy(huffman_tree_t* self);