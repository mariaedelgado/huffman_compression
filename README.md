Welcome to Maria's Huffman Compression program !

## How to run the program
The steps to make the program work are the following. More information about the code can be found in documentation/code_documentation.md.

1. Generate the executable(s). The method to compile used in class has been included in a Makefile so that including the source and include files is automatic. To do that, we will run **in the root folder**:

    `make`

2. Prepare the files to be used by dropping them in the folder **/data**. This has been done to easen the user's life - you don't need to specify the full path of your files !

3. Execute the program. You will see two executables have been generated, and you may want to use one or the other depending on your needs:

    - **huffman_compression**: this executable requires a one line command in the terminal, ideal if you only want to compress/decompress quickly. To run it:

        Compress: `./huffman_compression -c -i <input_file> -o <output_file>`

        Decompress: `./huffman_compression -d -i <input_file> -o <output_file>`

    - **huffman_compression_gui**: this executable runs an interactive terminal, in which the user will be asked to write the required inputs. Additionally, it is possible to print the Huffman Tree and Huffman Codes of the process. To run it:

        `./huffman_compression_gui`

---