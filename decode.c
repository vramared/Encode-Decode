# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <ctype.h>
# include "huffman.h"
# include "code.h"
# include "bv.h"
	
# define BITS 4096    /* These macros are used later within the code to read half a kilobyte at a time. */
# define BYTES 512	/* I multiplied 512 by 8 to get the right number of bits for the bit vector */

int main(int argc, char **argv)
{
	int c, fd = 0, fdOut = 0;                             /* integer value for getopt, file descriptors for input file and output file */
	bool verbose = false, showTree = false;			/* boolean conditions for printing verbose or the tree */

	while ((c = getopt(argc, argv, "i:o:vp")) != -1)   /* getopt loop, implemented similar to the past assignments */
	{
		switch (c)
		{
			case 'i':			
			{
				fd = open(optarg, O_RDONLY);   /* file descriptor is set according to the user's input */
				break;
			}
			case 'o':
			{
				fdOut = open(optarg, O_CREAT | O_EXCL | O_WRONLY, 0644);  /* output file is created if it does not already exist */
				break;
			}
			case 'v':
			{
				verbose = true;    /* verbose is turned on */
				break;
			}
			case 'p':
			{
				showTree = true;    /* printTree is turned on */
				break;
			}
			default :
			{
				break;		/*  default is just a break */
			}
			
		}
	} 

	if (fd == -1)
	{
		printf("File Not Found\n");    /* prints an error message if the file was not found */
		return 1;
	}
	
	if (fdOut == -1)
	{
		printf("Output file already exists\n");
		return 1;
	}

	uint32_t magicNum = 0;   /* value for the magic number */

	read(fd, &magicNum, 4);
			
	if (magicNum != 0xdeadd00d)   /* here, the first four bytes of the compressed file are read, and i check if it corresponds to the desired magic number */
	{
		printf("Not a compressed file\n");   /* error message is printed. Nothing is done afterwards */
		return 1;
	}	
	
	uint16_t treeSize = 0;  	/* variables for the tree size and the file size */
        uint64_t fileSize = 0;

	read(fd, &fileSize, 8);   /* the next 8 bytes are the file size */
        read(fd, &treeSize, 2);   /* the next 2 bytes give the tree size */
	
	uint8_t *savedTree = malloc(sizeof(uint8_t)*treeSize);	/* array is allocated for the size of the tree, to hold the dumpTree() values */
	read(fd, savedTree, treeSize);	/* The dumpTree output is read into the savedTree array */
	
	treeNode *tree = loadTree(savedTree, treeSize);	/* tree variable is set to the return value of load tree */
			
	treeNode *t = tree; /* temporary pointer to the root of the tree, used to step through the tree */
	
	uint8_t *buf = malloc(sizeof(uint8_t)*fileSize);  /* output buffer is allocated, for printing all the bytes at the end */
	uint64_t bytesDecoded = 0;  /* counter for the number of bytes decoded, so that it does not decode more than it needs to */

	while (bytesDecoded < fileSize)  /* runs until bytesDecoded is equal to File Size, so as to not emit extraneous symbols */
	{
		bitV *bits = newVec(BITS);  /* new vector of size 4096 which is how many bits there are in half a kilobyte */
		read(fd, bits->v, BYTES);
		
		for (uint32_t i = 0; i < bits->l; i++)  /* loops through each bit in the bit vector */
                {
                        if (valBit(bits, i) == 0)   /* if the bit is 0, the temporary pointer goes left */
			{
				t = t -> left;
			}
			else			/* otherwise the temporary pointer goes right */
			{
				t = t -> right;	
			}
			if (t -> leaf)	/* if the pointer is at a leaf */
			{
				buf[bytesDecoded] = t -> symbol;		/* the symble is added to the buffer */
				t = tree;			/* temp pointer returns to the root */ 
				bytesDecoded++;		/* bytes Decoded is incremented */
			}
			if (bytesDecoded == fileSize)		/* I check if i've decoded enough bytes, and then it breaks so that we have an exact replica 
									of the uncompressed file */
			{
				break;
			}
                }
		delVec(bits);  /* the vector is freed so that the next half kiloybyte can be read */
	}
	
	if (verbose)   /* If verbose is on, then the stats are printed */
	{
		printf("Original %lu bits: tree (%u)\n", fileSize*8, treeSize);
	}
	
	write(fdOut, buf, bytesDecoded); /* the decompressed file is printed to the output file */
	
	if (showTree)		/* if print Tree is on, the tree for the corresponding file is printed */
	{
		printTree(tree, 1);
	} 

	free(buf);		/* all the allocated memory is freed */
	free(savedTree);
	delTree(tree);

	close(fd);	/* the input and output files are close */
	close(fdOut);
	return 0;
}
