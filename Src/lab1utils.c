/*
 * lab1utils.c
 *
 *  Created on: Sep 15, 2022
 *      Author: Jimmy Sheng
 *      Format: BSD/Allman
 */

#include "lab1math.h"

void cMult(float *vec_a, float *vec_b, float *vec_dest, uint32_t vec_length)
{
	for (uint32_t i = 0; i < vec_length; i++)
	{
		vec_dest[i] = vec_a[i] * vec_b[i];
	}
}
