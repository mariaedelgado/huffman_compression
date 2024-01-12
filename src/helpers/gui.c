#include <libgen.h>
#include <string.h>

#include "../../include/helpers/gui.h"

#define MAX_SIZE_FILENAME   256

static void gui__clear_screen()
{
    printf("\e[1;1H\e[2J");
    return;
}

int gui__print_landing_page()
{    
    int ret = 0;
    char option = ' ';

    printf(" _____________________________\n");
    printf("| Huffman Compression Program |\n");
    printf("|           Welcome           |\n");
    printf("|=============================|\n");
    printf("| c = Compress file.          |\n");
    printf("| d = Decompress file.        |\n");
    printf("| e = Exit.                   |\n");
    printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    scanf("%c", &option);

    if (option == 'e') {
        ret = 0;
    }  else if (option == 'c') {
        ret = 1;
    } else if (option == 'd') {
        ret = 2;
    } else {
        ret = -1;
    }

    return ret;
}

int gui__print_compression_page(char* input_filepath, char* output_filepath)
{
    int ret = 0;

    gui__clear_screen();

    printf("Input file: \n");
    scanf("%s", input_filepath);

    printf("Output file. If not provided, '$(input_file)_compressed.txt': \n");
    scanf("%s", output_filepath);

    if (strcmp(output_filepath, "\n") == 0) {
        sprintf(output_filepath, "%s_compressed.txt", basename(input_filepath));
        printf("%s", output_filepath);
    }

    // raise error if filepath already exists

    return ret;
}

int gui__print_decompression_page(char* input_filepath, char* output_filepath)
{
    int ret = 0;

    gui__clear_screen();

    printf("Input file: \n");
    scanf("%s", input_filepath);

    printf("Output file. If not provided, '$(input_file)_decompressed.txt': \n");
    scanf("%s", output_filepath);

    if (strcmp(output_filepath, "\n") == 0) {
        sprintf(output_filepath, "%s_decompressed.txt", basename(input_filepath));
        printf("%s", output_filepath);
    }

    // raise error if filepath already exists

    return ret;
}


static float compute_compression_ratio(huffman_io_t* huffman_io)
{
    int input_file_size = 0;
    int output_file_size = 0;
    float compression_ratio = 0.0;

    // Compute input file size
    rewind(huffman_io->fh_in);
    fseek(huffman_io->fh_in, 0L, SEEK_END);
    input_file_size = ftell(huffman_io->fh_in);

    // Compute output file size
    rewind(huffman_io->fh_out);
    fseek(huffman_io->fh_out, 0L, SEEK_END);
    output_file_size = ftell(huffman_io->fh_out);

    // Print ratio
    compression_ratio = (float)output_file_size/(float)input_file_size;
    return compression_ratio;
}

// static int compute_number_of_levels(const huffman_codes_t* huffman_codes)
// {
//     int n_levels = 0;

//     for (int i = 0; i < 95; i++)
//     {
//         if (n_levels < huffman_codes->n_significant_bits[i])
//             n_levels = huffman_codes->n_significant_bits[i];
//     }

//     return n_levels;
// }

static void gui__print_node(const huffman_tree_t* huffman_tree, int current_index, int level)
{
    int n_spaces = 0;

    const node_t* current_node = &huffman_tree->node[current_index];
    const node_t* lchild_node = &huffman_tree->node[current_node->lchild_index];
    const node_t* rchild_node = &huffman_tree->node[current_node->rchild_index];

    if (is_leaf(lchild_node)) {
        if (lchild_node->character == '\000') {
            printf("----- EOF (%d) ", lchild_node->frequency);
        } else {
            printf("-----  %c  (%d) ", lchild_node->character, lchild_node->frequency);
        }
        
    } else {
        printf("----- Itn (%d) ", lchild_node->frequency);
        gui__print_node(huffman_tree, current_node->lchild_index, level + 1);
    }

    n_spaces = 11 + 14*(level - 1);

    if (is_leaf(rchild_node)) {
        printf("\n");
        for (int i = 0; i < n_spaces; i++) printf(" ");
        printf("|");
        if (rchild_node->character == '\000') {
            printf("\n");
            for (int i = 0; i < n_spaces; i++) printf(" ");
            printf("---- EOF (%d) ", rchild_node->frequency);
        } else {
            printf("\n");
            for (int i = 0; i < n_spaces; i++) printf(" ");
            printf("----  %c  (%d) ", rchild_node->character, rchild_node->frequency);
        }
    } else {
        printf("\n");
        for (int i = 0; i < n_spaces; i++) printf(" ");
        printf("---- Itn (%d) ", rchild_node->frequency);
        gui__print_node(huffman_tree, current_node->rchild_index, level + 1);
    }

    return;
}

static int gui__print_huffman_tree(const huffman_tree_t* huffman_tree, const huffman_codes_t* huffman_codes)
{
    int ret = 0;
    char option = ' ';

    gui__clear_screen();

    // Print the nodes, starting from root
    printf("Root (%d) ", huffman_tree->node[huffman_tree->number_of_nodes - 1].frequency);
    gui__print_node(huffman_tree, huffman_tree->number_of_nodes - 1, 1);

    printf("\n Press 'r' to Return\n");
    printf(" Press 'e' to Exit\n");
    scanf(" %c", &option);

    if (option == 'r') {
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}

static int gui__print_huffman_codes(const huffman_codes_t* huffman_codes)
{
    int ret = 0;
    char option = ' ';
    int spaces_after = 0;
    int spaces_before = 0;
    char spaces[14] = "              ";

    gui__clear_screen();

    printf(" ___________________________________\n");
    printf("| Character | ASCII  | Huffman code |\n");
    printf("|===================================|\n");

    for (int i = 0; i < 95; i++)
    {
        if (huffman_codes->n_significant_bits[i] == 0)
            continue;

        // Compute how many spaces before and after the code so that it fits the printed table
        spaces_before = (14 - huffman_codes->n_significant_bits[i])/2;
        spaces_after = 14 - spaces_before - huffman_codes->n_significant_bits[i];

        printf("|     %c     |   %d   |%.*s%.*s%.*s|\n", (char)(i + 32), i + 32,
                                                        spaces_before, spaces,
                                                        huffman_codes->n_significant_bits[i], huffman_codes->code[i],
                                                        spaces_after, spaces);
    }

    printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    printf(" Press 'r' to Return\n");
    printf(" Press 'e' to Exit\n");
    scanf(" %c", &option);

    if (option == 'r') {
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}

void gui__print_result_page(huffman_io_t* huffman_io, const huffman_tree_t* huffman_tree, const huffman_codes_t* huffman_codes)
{
    char option = ' ';

    gui__clear_screen();

    printf(" _____________________________\n");
    printf("| Huffman Compression Program |\n");
    printf("|           Results           |\n");
    printf("|=============================|\n");
    printf("| Compression ratio: %0.2f     |\n", compute_compression_ratio(huffman_io));
    printf("|                             |\n");
    printf("| t = Print Huffman Tree      |\n");
    printf("| c = Print Huffman Codes     |\n");
    printf("| e = Exit.                   |\n");
    printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    scanf(" %c", &option);

    if (option == 'e') {
        return;

    } else if (option == 't') {

        if (gui__print_huffman_tree(huffman_tree, huffman_codes) == 1) {
            gui__print_result_page(huffman_io, huffman_tree, huffman_codes);
        } else {
            return;
        }

    } else if (option == 'c') {
        if (gui__print_huffman_codes(huffman_codes) == 1) {
            gui__print_result_page(huffman_io, huffman_tree, huffman_codes);
        } else {
            return;
        }
    }
};

void gui__print_error()
{
    return;
}