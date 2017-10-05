# include "bv.h"
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>

/*
 * The following code defines all the functions within the header file "bv.h". 
 *
 * These are all the functions used for the bit vector data structure. 
 * A bit vector is just a long string of bits (1's and 0's) which can represent true (1) or false (0).
 *
 */

bitV *newVec(uint32_t length)
{
	bitV *newVec;
	newVec = malloc(sizeof(bitV));   /* memory is allocated for the bit vector itself */
	newVec -> l = length;    /* length variable is set */ 
	newVec -> v = calloc((length/8)+1, sizeof(uint8_t)); /* memory is allocated for each bit, and then set to a value of 0 */
	return newVec; 
}

void delVec(bitV *deleteVec)
{
	free(deleteVec -> v); /* the allocated memory using malloc and calloc are freed */
	free(deleteVec); /* first the bits are freed, then the memory allocated for the bit vector is freed */
	return;
}

void clrBit(bitV *vec, uint32_t k) /* k is the index of the bit which is to be cleared */
{
	(vec ->v[k >> 3]) &= ((0x1 << (k%8)) ^ (0xFF));  /* This sets the specified bit in the bit vector to 0. */
	return;
}

void oneVec(bitV *oneVec) /* This will set a specied bit vector to 1 for all values */
{
	for(uint32_t i = 0; i < (((oneVec ->l)/8)+1); i++) /* for loop traverse through each byte */
	{ 
		((oneVec -> v[i])) |= (~(oneVec -> v[i])); /* the entire bitVector is bitwise OR'ed with it's compliment to set all bits to 1*/
	}
	return;
}

void setBit(bitV *vec, uint32_t k) /* in the bit vector vec, the bit corresponding to index k is set to 1 */
{
	(vec->v[k >> 3]) |= (0x1<<(k%8)); 
	return;
}

uint8_t valBit(bitV *val, uint32_t k)  /* the value of the bit in bit Vector val at index k is returned */
{
	return (val -> v[k>>3] >> (k%8)) & (0x1);
} 

uint32_t lenVec(bitV *lengthVec) /* the length of the bit vector is returned */
{
	return lengthVec -> l;
}
 
