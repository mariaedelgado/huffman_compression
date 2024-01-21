# Code documentation
This document contains more information about Maria's Huffman Compression code !

## Folder structure

```bash
huffman_compression
|
├── data
|
├── documentation
|
├── executables
│   ├── huffman_compression_gui.c
│   └── huffman_compression.c
|
├── include
│   ├── helpers
│   └── huffman
|
├── src
│   ├── helpers
│   └── huffman
└──
```

### Data
Folder data is used to store the files to be used in the process. You will find two examples:

- a_room_of_ones_own.txt (X characters)
- lorem_ipsum.txt (X characters)
- Feel free to drop more files here !

### Documentation
In this folder, the project objectives document is found to learn more about the code.

### Executables
The main programs of the executables are found inside this folder. When compiling (Step 1 in the README.md), the executables are going to be generated inside of this folder.

We recall that:
- **huffman_compression**: this executable requires a one line command in the terminal, ideal if you only want to compress/decompress quickly. To run it:

    Compress: `./huffman_compression -c -i <input_file> -o <output_file>`

    Decompress: `./huffman_compression -d -i <input_file> -o <output_file>`

- **huffman_compression_gui**: this executable runs an interactive terminal, in which the user will be asked to write the required inputs. Additionally, it is possible to print the Huffman Tree and Huffman Codes of the process. To run it:

    `./huffman_compression_gui`

### Include and source
The functions used by the executables are found in here. These are:

- *helpers*
    - *bits*: contains functions used to read and write in binary the Huffman codes.
    - *gui*: contains functions used in **huffman_compression_gui.exe**, so print functions to show the GUI in the terminal and ask the user for the inputs.
    - *parseargs*: contains parser used in **hufman_compression.exe** to obtain the inputs from the command line.
- *huffman*
    - *huffman_codes*: contains functions to compute the Huffman codes from the Huffman tree (if we are compressing) or the header of the file (if we are decompressing).
    - *huffman_io*: contains input/output related functions, so reading and writing the input and output files of our program.
    - *huffman_tree*: functions to generate the Huffman tree.

## Other considerations

Throughout the code, you will probably see that most of the functions return an **int** as an error code. It works as follows:

- Returns 0 if OK
- Else, error