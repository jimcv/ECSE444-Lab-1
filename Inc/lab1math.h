/*
 * lab1math.h
 *
 *  Created on: Sep 13, 2022
 *      Author: Jimmy Sheng
 */

#ifndef INC_LAB1MATH_H_
#define INC_LAB1MATH_H_

#include "main.h"

void cMax(float *array, uint32_t size, float *max, uint32_t *maxIndex);

extern void asmMax(float *array, uint32_t size, float *max, uint32_t *maxIndex);

void cMult(float *vec_a, float *vec_b, float *vec_dest, uint32_t vec_length);



#endif /* INC_LAB1MATH_H_ */
