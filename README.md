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

## Examples

A few examples have been prepared in **data/** folder to test the algorithm. Feel free to try your own files !

### Test 1

A basic example of compression and decompression with a small file. This is a cool example to test because, since it's small, the Huffman Tree can be displayed in terminal with the GUI !

- Input file for the compression program: `example.txt`
- Input file for the decompression program: `example_compressed.bin` (or you can also use the result from the compression).

Expected results:

|               | Input file size | Output file size | Compression ratio |
|---------------|------------|-------------|-------|
| Compression   | 20 | 14 | 0.7 |
| Decompression | 14 | 20 | - |

### Test 2

Now, we can try with a much bigger file !

- Input file for the compression program: `lorem_ipsum.txt`
- Input file for the decompression program: `lorem_ipsum_compressed.bin` (or you can also use the result from the compression).

Expected results:

|               | Input file size | Output file size | Compression ratio |
|---------------|------------|-------------|-------|
| Compression   | 67793 | 36106 | 0.532591879 |
| Decompression | 36106 | 67793 | - |