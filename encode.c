# include <stdint.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "huffman.h"
# include "queue.h"
# include "bv.h"
# include "code.h"

int main(int argc, char **argv)
{

	int inputFile = 0;
	int outputFile = 1;
	uint8_t printTreeFlag = 0;
	uint8_t verboseFlag = 0;
	int c = 0;
	if (argc == 1)	// if no arguments are passed in print error message
	{
		printf("error specify an input file\n");
		return 1;
	}
	while ((c = getopt(argc, argv, "i:o:vp")) != -1)	//reused structure from assignment3
	{
			
		switch (c)
		{
			case 'i':	//specifies input file
			{
				inputFile = open(optarg, O_RDONLY);
				break;
			}
			case 'o':	//specifies outputFile
			{
				outputFile = open(optarg,O_CREAT | O_EXCL | O_WRONLY,0644);
				break;
			}
			case 'v':	//verbose mode
			{
				verboseFlag = 1;
				break;
			}
			case 'p':	// print tree mode
			{
				printTreeFlag = 1;
				break;
			}
			default :
			{
				break;
			}

		}
	}
	if (inputFile == -1)
	{
		printf(" error input file not found\n");
		return 1;
	}
	
	if (outputFile == -1)
	{
		printf("error output file already exists\n");
		return 1;
	}
	struct stat fileStat;	//http://codewiki.wikidot.com/c:system-calls:fstat

	if (fstat(inputFile,&fileStat) < 0)  //if fstat fails print error message
	{
		printf("fstat error");
		return 1;
	}
	
	uint32_t histogram[256];	//histogram for frequency of each byte
	memset(histogram, 0x0, sizeof(histogram));	//initialize histogram to all zeros
	histogram[0] = 0x01;	//sets histogram[0] to 1 to prevent empty tree
	histogram[255] = 0x01;	//sets histogram[0] to 1 to prevent empty tree
	uint8_t *inputBuffer = calloc(fileStat.st_size, sizeof(uint8_t));	//creates an input buffer for reading in the file
	read(inputFile,inputBuffer, fileStat.st_size);	//reads file into input buffer
	for (int i = 0; i<fileStat.st_size; i++)	//iterates through file and increments numbytes corresponding element of input Buffer
	{
		histogram[inputBuffer[i]]++;
		
	}
	
	queue *histogramQueue = newQueue(768);	//queue for creation of huffman tree, 768 recommended by jurik
	
	for (uint16_t i = 0; i < 256; i++)	//iterates through every byte combination
	{
		if (histogram[i])	//if present in histogram enqueue a newNode
		{
			enqueue(histogramQueue, newNode(i, 1, histogram[i]));
		}
	}
	treeNode *itemA = NIL;
	treeNode *itemB = NIL;
	
	// creating a huffman tree
	while (empty(histogramQueue) == 0 )	//while the queue is not empty
	{
		dequeue(histogramQueue, &itemA);	//dequeue an item
		if (empty(histogramQueue))	// if empty, break
		{
			break;
		}
		dequeue(histogramQueue, &itemB);	//dequeue another item 
		enqueue(histogramQueue,join(itemA, itemB));	//enqueue the two dequeued item 
	
	}
	// create a code table
	code s = newCode();
	code codeTable[256];
	buildCode(itemA, s, codeTable);
	
	//outputBuffer
	bitV *outputBuffer = newVec(fileStat.st_size*8);
	uint32_t index = 0;

	for (int i = 0; i < fileStat.st_size; i++)	//iterate through the file size
	{
		for (uint32_t j = 0; j < codeTable[inputBuffer[i]].l;j++)	//reused code from bv.c, iterates through code array
		{
			if ((codeTable[inputBuffer[i]].bits[j>>3] >> (j%8)) & (0x1))	//if code the value is 1 in the code table
			{
				setBit(outputBuffer, index + j);	//append the corresponding bit
			}
		}
		index += codeTable[inputBuffer[i]].l;	//increase the index by the bits added
	}

	write(outputFile, "\x0D\xD0\xAD\xDE", 4);	//magic number
	write(outputFile, &fileStat.st_size, sizeof(uint64_t));	//file size
	uint16_t huffmanTreeSize = 0;	
	for (int i = 0; i < 256;i++)	//iterates from 0 to 256 if value is present in histogram then iterate huffmanTreeSize
	{
		if (histogram[i])
		{
			 huffmanTreeSize++;
		}
	}
	
	uint16_t treeSize = (huffmanTreeSize*3) - 1;	//from professor's post on piazza 
	write(outputFile, &treeSize, sizeof(uint16_t));	//writes huffman tree size
	dumpTree(itemA, outputFile);
	write(outputFile, outputBuffer->v, (index+7)/8);	//number of  bytes in the encoded file
	if (verboseFlag)	//checks verboseFlag
	{
		printf("Original %ld bits: leaves %d (%d bytes) encoding %d bits (%.4f%%).\n",fileStat.st_size*8,huffmanTreeSize, treeSize,index,( (float) index / ((float) fileStat.st_size*8) )*100);
	}
	if (printTreeFlag)      //checks printTreeFlag
        {
                printTree(itemA,1);
        }	
	//free allocated memory
	delQueue(histogramQueue);
	delTree(itemA);
	delVec(outputBuffer);
	free(inputBuffer);
	
	close(inputFile);
	close(outputFile);

	return 0;
}
