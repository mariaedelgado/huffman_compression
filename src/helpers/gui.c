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

    printf(" ___________________________________________\n");
    printf("|         Huffman Compression Program       |\n");
    printf("|                   Welcome                 |\n");
    printf("|===========================================|\n");
    printf("| c = Compress file.                        |\n");
    printf("| d = Decompress file.                      |\n");
    printf("| any other key = Exit.                     |\n");
    printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    printf("| ");
    scanf(" %c", &option);

    if (option == 'c') {
        ret = 1;
    }  else if (option == 'd') {
        ret = 2;
    } else {
        return 0;
    }

    return ret;
}

static void get_basename(char* filepath, char* basename)
{
    uint8_t pos = 0;

    while (pos < MAX_SIZE_FILENAME)
    {
        if (filepath[pos] == '.') {
            break;
        } else {
            basename[pos] = filepath[pos];
        }

        pos++;
    }

    return;
}

int gui__print_compression_page(char* input_filepath, char* output_filepath)
{
    int ret = 0;
    char option = ' ';
    char tmp_input_filepath[100] = { 0 };
    char tmp_output_filepath[100] = { 0 };

    gui__clear_screen();

    printf(" ____________________________________________\n");
    printf("| Please leave your files in  data/ folder   |\n");
    printf("|============================================|\n");
    printf("| Provide file to compress:                  |\n");
    printf("| ");
    scanf("%s", tmp_input_filepath);
    printf("|                                            |\n");
    printf("| Do you want to provide an output filepath? |\n");
    printf("| (y/n)                                      |\n");
    printf("| *if not, default name will be given        |\n");
    printf("| ");
    scanf(" %c", &option);
    printf("|                                            |\n");

    sprintf(input_filepath, "data/%s", tmp_input_filepath);

    if ((option == 'y') || (option == 'Y')) {
        printf("| Please provide output:                     |\n");
        printf("| ");
        scanf(" %s", tmp_output_filepath);
        sprintf(output_filepath, "data/%s", tmp_output_filepath);
        printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    } else {
        char basename[50] = { 0 };
        get_basename(tmp_input_filepath, basename);
        sprintf(output_filepath, "data/%s_compressed.bin", basename);
        printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }
    
    return ret;
}

int gui__print_decompression_page(char* input_filepath, char* output_filepath)
{
    int ret = 0;
    char option = ' ';
    char tmp_input_filepath[100] = { 0 };
    char tmp_output_filepath[100] = { 0 };

    gui__clear_screen();

    printf(" ____________________________________________\n");
    printf("| Please leave your files in  data/ folder   |\n");
    printf("|============================================|\n");
    printf("| Provide file to decompress:                |\n");
    printf("| ");
    scanf("%s", tmp_input_filepath);
    printf("|                                            |\n");
    printf("| Do you want to provide an output filepath? |\n");
    printf("| (y/n)                                      |\n");
    printf("| *if not, default name will be given        |\n");
    printf("| ");
    scanf(" %c", &option);
    printf("|                                            |\n");

    sprintf(input_filepath, "data/%s", tmp_input_filepath);

    if ((option == 'y') || (option == 'Y')) {
        printf("| Please provide output:                     |\n");
        printf("| ");
        scanf(" %s", tmp_output_filepath);
        sprintf(output_filepath, "data/%s", tmp_output_filepath);
        printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    } else {
        char basename[50] = { 0 };
        get_basename(tmp_input_filepath, basename);
        sprintf(output_filepath, "data/%s_decompressed.txt", basename);
        printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }

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

static void gui__print_node(const huffman_tree_t* huffman_tree, int current_index, int level)
{
    int n_spaces = 0;

    const node_t* current_node = &huffman_tree->node[current_index];
    const node_t* lchild_node = &huffman_tree->node[current_node->lchild_index];
    const node_t* rchild_node = &huffman_tree->node[current_node->rchild_index];

    if (is_leaf(lchild_node)) {
        if (lchild_node->character == '\000') {
            printf("---- EOF (%d) ", lchild_node->frequency);
        } else {
            printf("----  %c  (%d) ", lchild_node->character, lchild_node->frequency);
        }
        
    } else {
        printf("---- Itn (%d) ", lchild_node->frequency);
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

    printf(" ____________________________________________\n");
    printf("|         Huffman Compression Program       |\n");
    printf("|                    Tree                   |\n");
    printf("|===========================================|\n");

    if (huffman_tree->number_of_nodes > 20) {
        printf("| Tree is too big to be displayed.          |\n");
        printf("| Please, check the codes instead.          |\n");
    } else {
        // Print the nodes, starting from root
        printf("Root (%d) ", huffman_tree->node[huffman_tree->number_of_nodes - 1].frequency);
        gui__print_node(huffman_tree, huffman_tree->number_of_nodes - 1, 1);
    }

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    printf(" r = Return\n");
    printf(" any other key = Exit\n");

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

    printf(" ____________________________________________\n");
    printf("|         Huffman Compression Program       |\n");
    printf("|                    Codes                  |\n");
    printf("|===========================================|\n");
    printf(" ___________________________________________\n");
    printf("    | Character | ASCII  | Huffman code |\n");
    printf("    |===================================|\n");

    for (int i = 0; i < 95; i++)
    {
        if (huffman_codes->n_significant_bits[i] == 0)
            continue;

        // Compute how many spaces before and after the code so that it fits the printed table
        spaces_before = (14 - huffman_codes->n_significant_bits[i])/2;
        spaces_after = 14 - spaces_before - huffman_codes->n_significant_bits[i];

        printf("    |     %c     |   %d   |%.*s%.*s%.*s|\n", (char)(i + 32), i + 32,
                                                        spaces_before, spaces,
                                                        huffman_codes->n_significant_bits[i], huffman_codes->code[i],
                                                        spaces_after, spaces);
    }

    printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    printf(" r = Return\n");
    printf(" any other key = Exit\n");
    scanf(" %c", &option);

    if (option == 'r') {
        ret = 1;
    } else {
        ret = 0;
    }

    return ret;
}

void gui__print_result_page_compressed(huffman_io_t* huffman_io, const huffman_tree_t* huffman_tree, const huffman_codes_t* huffman_codes)
{
    char option = ' ';

    gui__clear_screen();

    printf(" ____________________________________________\n");
    printf("|         Huffman Compression Program       |\n");
    printf("|                   Results                 |\n");
    printf("|===========================================|\n");
    printf("| Compression ratio: %0.2f                   |\n", compute_compression_ratio(huffman_io));
    printf("|                                           |\n");
    printf("| t = Print Huffman Tree                    |\n");
    printf("| c = Print Huffman Codes                   |\n");
    printf("| any other key = Exit                      |\n");
    printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    scanf(" %c", &option);

    if (option == 't') {

        if (gui__print_huffman_tree(huffman_tree, huffman_codes) == 1) {
            gui__print_result_page_compressed(huffman_io, huffman_tree, huffman_codes);
        } else {
            return;
        }

    } else if (option == 'c') {
        if (gui__print_huffman_codes(huffman_codes) == 1) {
            gui__print_result_page_compressed(huffman_io, huffman_tree, huffman_codes);
        } else {
            return;
        }
    } else {
        return;
    }
};

void gui__print_result_page_decompressed(huffman_io_t* huffman_io) 
{
    char option = ' ';

    gui__clear_screen();

    printf(" ____________________________________________\n");
    printf("|       Huffman De-Compression Program      |\n");
    printf("|                   Results                 |\n");
    printf("|===========================================|\n");
    printf("| Compression ratio: %0.2f                   |\n", compute_compression_ratio(huffman_io));
    printf("|                                           |\n");
    printf("| Press any key to exit                     |\n");
    printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    scanf(" %c", &option);
    return;
}

int gui__print_error()
{
    char option = ' ';

    gui__clear_screen();

    printf(" ____________________________________________\n");
    printf("|                    ERROR                   |\n");
    printf("|============================================|\n");
    printf("| Input file problem. Please, make sure:     |\n");
    printf("|   - File is inside data folder             |\n");
    printf("|   - Filename is written correctly          |\n");
    printf("|                                            |\n");
    printf("| r = Return                                 |\n");
    printf("| any other key = Exit.                      |\n");
    printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    scanf(" %c", &option);

    if (option == 'r') {
        return 1;
    } else {
        return 0;
    }
}