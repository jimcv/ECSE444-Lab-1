/*
 * cmax.c
 *
 *  Created on: Sep 13, 2022
 *      Author: Jimmy Sheng
 *      Format: BSD/Allman
 */

#include "lab1math.h"

void cMax(float *array, uint32_t size, float *max, uint32_t *maxIndex)
{
	// initially set max to start of array
	(*max) = array[0];
	(*maxIndex) = 0;
	for (uint32_t i = 1; i < size; i++)
	{
		if (array[i] > (*max))
		{
			(*max) = array[i];
			(*maxIndex) = i;
		}
	}
}
