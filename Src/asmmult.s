/*
 * asmmult.s
 *
 *  Created on: Sep 16, 2022
 *      Author: Jimmy Sheng
 */

.syntax unified  // mixed 16-bit and 32-bit instructions

.global asmMult  // export label asmMult

.section .text.rodata

/*
 void asmMult(float *vec_a, float *vec_b, float *vec_dest, uint32_t vec_length);
 
 R0 = ptr vec_a
 R1 = ptr vec_b
 R2 = ptr vec_dest
 R3 = vec_length
 */

asmMult:
	PUSH			{R4}
	MOV				R4, #0				// R4 <- current index

loop:
	CMP				R4, R3
	BGE				done				// finished when index >= vec_length
	VLDR.f32		S0, [R0]
	VLDR.f32		S1, [R1]
	VMUL.f32		S0, S0, S1
	VSTR.f32		S0, [R2]
	// increment index
	ADD				R4, R4, #1
	ADD				R0, R0, #4			// R0 <- *vec_a[i]
	ADD				R1, R1, #4			// R1 <- *vec_b[i]
	ADD				R2, R2, #4			// R2 <- *vec_dest[i]
	B				loop

done:
	POP				{R4}
	BX				LR


