#include "../include/huffman/huffman_tree.h"

const int huffman_tree__add_new_leaf(huffman_tree_t* self, char character)
{
    int ret = -1;

    if (self == NULL)
    {
        goto end;
    }

    // Increase the counter of number of nodes and resize the array.
    self->number_of_nodes++;
    void* tmp = realloc(self->node, self->number_of_nodes*sizeof(node_t));
    if (tmp == NULL)  {
        goto end;
    } else {
        self->node = tmp;
    }

    // Add information about new node in the correct index of the array.
    self->node[self->number_of_nodes - 1].character = character;
    self->node[self->number_of_nodes - 1].frequency = 1;
    self->node[self->number_of_nodes - 1].lchild_index = -1;
    self->node[self->number_of_nodes - 1].rchild_index = -1;

    ret = 0;

end:
    return ret;
}

int huffman_tree__store_character(huffman_tree_t* self, char character)
{
    int ret = -1;

    if (self == NULL)
    {
        goto end;
    }

    // Iterate all the stored nodes. If already present, increase the
    // number of ocurrences of node 'c'.
    for (size_t i = 0; i < self->number_of_nodes; i++)
    {
        if (character == self->node[i].character) 
        {
            self->node[i].frequency++;
            ret = 0;
            goto end;
        }
    }

    // If node was not present, add it.
    ret = huffman_tree__add_new_leaf(self, character);

end:
    return ret;
}

int huffman_tree__add_eof(huffman_tree_t* self)
{
    int ret = 0;

    if (self == NULL) {
        ret = -1;
        goto end;
    }

    ret = huffman_tree__add_new_leaf(self, '\0');

end:
    return ret;
}

static int huffman_tree__swap_character_index(node_t* node_i, node_t* node_min)
{
    int ret = -1;
    node_t tmp = *node_i;

    if ((node_i == NULL) || (node_min) == NULL)
    {
        goto end;
    }

    *node_i = *node_min;
    *node_min = tmp;
    ret = 0;

end:
    return ret;
}

int huffman_tree__sort(huffman_tree_t* self)
{
    int ret = 0;
    int min_index = 0;

    if (self == NULL)
    {
        ret = -1;
        goto end;
    }

    for (int i = 0; i < self->number_of_nodes - 1; i++)
    {
        min_index = i;

        for (int j = i + 1; j < self->number_of_nodes; j++)
        {
            if (self->node[j].frequency < self->node[min_index].frequency)
                min_index = j;
        }

        huffman_tree__swap_character_index(&self->node[i], &self->node[min_index]);
    }

end:
    return ret;
}

bool is_leaf(const node_t* node)
{
    return ((node->lchild_index == -1) && (node->rchild_index == -1)) ? true : false;
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
    self->node[itn_index].lchild_index = left_index;
    self->node[itn_index].rchild_index = right_index;

    // Move the next nodes one position up
    memcpy(&self->node[itn_index + 1], &tmp[itn_index], (self->number_of_nodes - itn_index - 1)*sizeof(node_t));

    free(tmp);

    for (int i = 0; i < self->number_of_nodes; i++)
    {
        if (is_leaf(&self->node[i]) == true) {
            printf("[%u] character: %c Frequency: %u\n", i, self->node[i].character, self->node[i].frequency);
        } else {
            printf("[%u] ITN node Frequency: %u\n", i, self->node[i].frequency);
        };
    }
    return 0;
}

int huffman_tree__generate(huffman_tree_t* self)
{
    int ret = -1;
    int left_index = 0;
    int right_index = 0;
    int index_read = 0;

    if (self == NULL)
        goto end;

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

int huffman_tree__destroy(huffman_tree_t* self)
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