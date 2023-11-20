CFLAGS = -g -Wall
INCLUDE_DIRECTORIES = -I/mnt/d/Code/huffman_compression/include

# make build with generate the executables for compress_file and decompress_file, and store the resulting .o
# the 'executables' folder. 
build: executables/compress_file.c executables/decompress_file.c
	gcc $(CFLAGS) $(INCLUDE_DIRECTORIES) -o executables/compress_file.o executables/compress_file.c src/*.c
	gcc $(CFLAGS) $(INCLUDE_DIRECTORIES) -o executables/decompress_file.o executables/decompress_file.c src/*.c

# make clean removes all the generated executables inside the 'executables' folder
clean:
	$(RM) executables/*.o
	