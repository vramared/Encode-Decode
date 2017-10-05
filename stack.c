# include "stack.h"
# include <stdio.h>
# include <stdlib.h>

/* The following is the implementation of the stack data structure, which is Last in First Out. */

stack *newStack()
{
	stack *s = (stack *) malloc(sizeof(stack)); /* first I allocate for the struct itself */
	s -> size = 0;	/* size is initially set to 0 */
	s -> top = 0;	/* top is also initially 0 */
	s -> entries = calloc(0, sizeof(item));	/* here I allocate for the entries within the array for the stack */
	return s;
}

void delStack(stack *s)		/* frees any memory allocated for the stack data structure */
{
	free(s -> entries);
	free(s);
	return;
}

bool stackEmpty(stack *s)	/* checks if the stack is empty. Returns true if it is, false if it isn't */
{
	if (s -> top == 0)
	{
		return true;
	}
	return false;
}

bool stackFull(stack *s)	/* checks if the stack is full, returns true if it is, false if it isnt */
{
	if (s -> size == s -> top)
	{
		return true;
	}
	return false;
}

item pop(stack *s)  /* pops an element from the stack, and returns the element which was popped */
{
	if (!stackEmpty(s))	/* it will only pop if there is something to pop in the stack */
	{		
		return s -> entries[--(s -> top)]; 	/* returns the element being popped, as well as decrementing the top variable */
	}
	return NULL;  /* if the stack is empty, then NULL is returned */
}

void push(stack *s, item i)  /* pushes a treeNode onto the stack */
{
	if (s -> size == 0)	/* if the size is 0 initially, then the array is reallocated to hold one treeNode */
	{
		(s -> size)++;	
		s -> entries = realloc(s -> entries, (s -> size)*(sizeof(item)));
	}
	else if (stackFull(s))	/* if the stack is full, the stack is doubled in size, and reallocated */
	{
		(s -> size) *= 2;
		s -> entries = realloc(s -> entries, (s -> size)*(sizeof(item))); 
	}
	s -> entries[(s -> top)++] = i;		/* the item is added to the stack. the top is incremented to the next position in the stack */
	return;
}
