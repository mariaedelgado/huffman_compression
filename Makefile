CFLAGS = -g -Wall
INCLUDE_DIRECTORIES = -I/mnt/d/Code/huffman_compression/include

# make build with generate the executables for huffman_compression, and store the resulting .o the 'executables' folder. 
build: executables/huffman_compression.c
	gcc $(CFLAGS) $(INCLUDE_DIRECTORIES) -o executables/huffman_compression.o executables/huffman_compression.c src/huffman/*.c src/helpers/*.c
	gcc $(CFLAGS) $(INCLUDE_DIRECTORIES) -o executables/huffman_compression_gui.o executables/huffman_compression_gui.c src/huffman/*.c src/helpers/*.c

# make clean removes all the generated executables inside the 'executables' folder
clean:
	$(RM) executables/*.o
	