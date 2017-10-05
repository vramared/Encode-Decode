CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
OBJECTSD = huffman.o stack.o queue.o decode.o bv.o
OBJECTSE = huffman.o stack.o queue.o encode.o bv.o

.PHONY  :       all     clean

all     :       decode encode
decode :       $(OBJECTSD)
	$(CC) $(CFLAGS) $(OBJECTSD) -o decode
decode.o :	decode.c
	$(CC) $(CFLAGS) -c decode.c
encode :       $(OBJECTSE)
	$(CC) $(CFLAGS) $(OBJECTSE) -o encode
encode.o :	encode.c
	$(CC) $(CFLAGS) -c encode.c
bv.o	:	bv.c
	$(CC) $(CFLAGS) -c bv.c
huffman.o :       huffman.c
	$(CC) $(CFLAGS) -c huffman.c
stack.o :       stack.c
	$(CC) $(CFLAGS) -c stack.c
queue.o :       queue.c
	$(CC) $(CFLAGS) -c queue.c
clean   :
	rm -f $(OBJECTSD) encode.o encode decode 
