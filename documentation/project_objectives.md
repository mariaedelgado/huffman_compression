Project assignment for IP405E Programming and C Language (ENAC- ASNAT23)

# Assignment 4: Data Compression

**Context:**

A data stream consists of a stream of symbols from a finite alphabet. Huffman coding is a particular type of optimal prefix code that is commonly used for lossless data compression. It was developed by David A. Huffman in 1952 during its PhD. Huffman coding forms the basis of the “zip” file format. The main properties of Huffman encoding are:

- It has a variable length. Frequent symbols are encoded with shorter code.
- It is a prefix code. No code occurs as a prefix of another code.
- It produces the shortest prefix code for a given symbol frequency.
  
Huffman coding uses a method for choosing the representation for each symbol, resulting in a prefix code. Huffman coding is such a widespread method for creating prefix codes that the term "Huffman code" is widely used as a synonym for "prefix code" even when such a code is not produced by Huffman's algorithm. The procedure to generate the code involves constructing a tree that proceeds as follows:

- Compute frequency of occurrence of each alphabet symbol.
- Sort all the different symbols and their particular frequency or probability.
- Take the two lowest probability symbols and create a new common node with the
probability equal to the sum of the two probability. Always make sure to add the lowest
summing up nodes.
- Add the new node to the table instead of the lowest two symbols or nodes.
- Repeat steps two and three until one symbol or node is remaining.
- Then allocate ‘0’ to all the right branches and ‘1’ to all right branches.
- Read the bits from the top of the tree to the bottom of each symbol and record their
particular bit pattern

**Objectives:**

The objective of the assignment is to understand and implement the full process of Huffman coding.
The project will be composed of the following tasks:
- Compute and build the Huffman compression tree from the file to be compressed
- Encode the file using that encoding tree
- Decode the encoded file to retrieve the initial file
- Compute the compression ratio
