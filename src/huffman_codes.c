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

int huffman_codes__encode_file(FILE* fh_in, FILE* fh_out, const huffman_codes_t* huffman_codes)
{
    int ret = -1;

    if ((fh_in != NULL) || (fh_out != NULL))
        goto end;

    // Rewind fh_in, since it has already been read
    rewind(fh_in);

    // Read the file character by character and print the respective Huffman code for the
    // given character in the output file.
    char c = ' ';
    int huffman_code = 0;
    // int n_significant_bits = huffman_codes->n_significant_bits;

    while (true)
    {
        c = fgetc(fh_in);
        huffman_code = huffman_codes->code[(int)c];

        // fprintf(fh_out, huffman_code);

        if (feof(fh_in)) {
            break;
        }
    }

    ret = 0;

end:
    return ret;
}