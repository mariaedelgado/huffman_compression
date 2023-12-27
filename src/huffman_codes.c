#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/huffman_codes.h"

// This is the ASCII table of printable characters:
// Character	Hex	Decimal	  	Character	Hex	Decimal	  	Character	Hex	Decimal
//       	     20	  32	 	    @	    40	  64	 	    `	     60	  96
//      !	     21	  33	 	    A	    41	  65	 	    a	     61	  97
//      "	     22	  34	 	    B	    42	  66	 	    b	     62	  98
//      #	     23	  35	 	    C	    43	  67	 	    c	     63	  99
//      $	     24	  36	 	    D	    44	  68	 	    d	     64	 100
//      %	     25	  37	 	    E	    45	  69	 	    e	     65	 101
//      &	     26	  38	 	    F	    46	  70	 	    f	     66	 102
//      '	     27	  39	 	    G	    47	  71	 	    g	     67	 103
//      (	     28	  40	 	    H	    48	  72	 	    h	     68	 104
//      )	     29	  41	 	    I	    49	  73	 	    i	     69	 105
//      *	     2a	  42	 	    J	    4a	  74	 	    j	     6a	 106
//      +	     2b	  43	 	    K	    4b	  75	 	    k	     6b	 107
//      ,	     2c	  44	 	    L	    4c	  76	 	    l	     6c	 108
//      -	     2d	  45	 	    M	    4d	  77	 	    m	     6d	 109
//      .	     2e	  46	 	    N	    4e	  78	 	    n	     6e	 110
//      /	     2f	  47	 	    O	    4f	  79	 	    o	     6f	 111
//      0	     30	  48	 	    P	    50	  80	 	    p	     70	 112
//      1	     31	  49	 	    Q	    51	  81	 	    q	     71	 113
//      2	     32	  50	 	    R	    52	  82	 	    r	     72	 114
//      3	     33	  51	 	    S	    53	  83	 	    s	     73	 115
//      4	     34	  52	 	    T	    54	  84	 	    t	     74	 116
//      5	     35	  53	 	    U	    55	  85	 	    u	     75	 117
//      6	     36	  54	 	    V	    56	  86	 	    v	     76	 118
//      7	     37	  55	 	    W	    57	  87	 	    w	     77	 119
//      8	     38	  56	 	    X	    58	  88	 	    x	     78	 120
//      9	     39	  57	 	    Y	    59	  89	 	    y	     79	 121
//      :	     3a	  58	 	    Z	    5a	  90	 	    z	     7a	 122
//      ;	     3b	  59	 	    [	    5b	  91	 	    {	     7b	 123
//      <	     3c	  60	 	    \	    5c	  92	 	    |	     7c	 124
//      =	     3d	  61	 	    ]	    5d	  93	 	    }	     7d	 125
//      >	     3e	  62	 	    ^	    5e	  94	 	    ~	     7e	 126
//      ?	     3f	  63	 	    _	    5f	  95	 	    Delete	 7f	 127


static int huffman_codes__generate_code(huffman_codes_t* self, const huffman_tree_t huffman_tree, int current_index, char* codes, int level)
{
    int ret = -1;
    int pos = 0;

    const node_t* current_node = &huffman_tree.node[current_index];
    const node_t* lchild_node = &huffman_tree.node[current_node->lchild_index];
    const node_t* rchild_node = &huffman_tree.node[current_node->rchild_index];

    if (is_leaf(lchild_node)) {
        codes[level - 1] = '0';
        pos = lchild_node->character - 32;
        self->n_significant_bits[pos] = level;
        strcpy(self->code[pos], codes);
        printf("Character: %c [%.*s]\n", lchild_node->character, level, self->code[pos]);
    } else {
        codes[level - 1] = '0';
        huffman_codes__generate_code(self, huffman_tree, current_node->lchild_index, codes, level + 1);
    }

    if (is_leaf(rchild_node)) {
        codes[level - 1] = '1';
        pos = rchild_node->character - 32;
        self->n_significant_bits[pos] = level;
        strcpy(self->code[pos], codes);
        printf("Character: %c [%.*s]\n", rchild_node->character, level, self->code[pos]);
    } else {
        codes[level - 1] = '1';
        huffman_codes__generate_code(self, huffman_tree, current_node->rchild_index, codes, level + 1);
    }

    return ret;
}

int huffman_codes__generate(huffman_codes_t* self, const huffman_tree_t huffman_tree)
{
    int ret = -1;
    int level = 1;  // Number of significant bits on each code
    char codes[MAX_TREE_HT] = { 0 };

    if (self == NULL)
        goto end;
    
    // Generate codes that are, for the moment, stored in the tree itself (TO BE ENHANCED)
    ret = huffman_codes__generate_code(self, huffman_tree, huffman_tree.number_of_nodes - 1, codes, level);

end:
    return ret;
}