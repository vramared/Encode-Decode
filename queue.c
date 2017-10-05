# include "queue.h"
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>

/* The following is the implementation of the priority queue, according to the queue data structure */

/* I wrote a swap function for when I sort the items in the priority queue */
 
/* I implemented the priority queue in such a way that it removes from the end which will always be on the left, and adds to the beginning, which will always be on the right. */

void swap(queue *q, uint32_t indexOne, uint32_t indexTwo)
{
	treeNode *temp = q -> Q[indexOne]; /* swaps the items at index one and index Two */
        q -> Q[indexOne] = q -> Q[indexTwo];
        q -> Q[indexTwo] = temp;
	return;
}

queue *newQueue(uint32_t qSize)
{
	queue *q = (queue *) malloc(sizeof(queue));  /* allocates memory for the struct itself */
	q -> size = qSize;	/* sets the size of the queue */
	q -> Q = calloc(qSize, sizeof(item));	/* allocates the array of the queue, using the size parameter */
	q -> head = 0;	/* head and tail of the queue begin at 0 */
	q -> tail = 0; 
	return q;	
}

void delQueue(queue *q)	/* frees any memory allocated by the queue structure */
{
	free(q -> Q);
	free(q);
	return;
}

bool empty(queue *q)	/* checks if the queue is empty, returns true if it is, and false otherwise */
{
	if (q -> tail == q -> head)
	{
		return true;
	}	
	return false;
}

bool full(queue *q)	/* checks if a queue is full, returns true if it is, and false otherwise */
{	
	if (q -> head == q -> size)
	{
		return true;
	}
	return false;
}

bool dequeue(queue *q, item *i)	/* dequeue removes an item from the queue */
{
	if (empty(q))	/* it will only dequeue if there is an item to dqueue in the first place */
	{
		return false; 
	}	
	*i = q -> Q[q -> tail];	/* sets the pointer which is passed in, so that that we can still access the item being dequeued */
	(q -> tail)++;	/* tail is incremented */
	return true;
}

bool enqueue(queue *q, item i)
{
	if (full(q))	/* it will not enqueue if the queue is full */
	{
		return false;
	}
	uint32_t tracker = (q -> head);	/* tracks the head value */
	
	q -> Q[q -> head] = i;	/* adds the item to the end */
	if (q -> head > 0) 	/* if there is at least one item already in the queue, it will check and insert the item at the correct position */
	{
		for (uint32_t i = tracker; i > (q -> tail); i--)	/* starts at the beginning, goes to the end, and swaps until the item to the left is no longer lesser than it */
		{	
			if ((q -> Q[tracker]) -> count < (q -> Q[i-1]) -> count)
			{
				swap(q, i-1, tracker);	/* swaps accordingly */
                		tracker = i-1;
			}
		}
	}

	(q -> head)++;	/* head of the queue is incremented, so a new item can be placed */
	return true;

}
