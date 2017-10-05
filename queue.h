# ifndef _QUEUE_H
# define _QUEUE_H
# include <stdint.h>
# include <stdbool.h>
# include "huffman.h"

/* Header file for the queue data structure */

typedef treeNode *item;

typedef struct queue
{
	uint32_t size;
	uint32_t head, tail;
	item *Q;
} queue;

queue *newQueue(uint32_t size);
void delQueue(queue *q);

bool empty(queue *q);
bool full(queue *q);

bool enqueue(queue *q, item i);
bool dequeue(queue *q, item *i);

# endif

