/*
 * asmmax.s
 *
 *  Created on: Sep 14, 2022
 *      Author: Jimmy Sheng
 */

//using a mix of 16-bit and 32-bit instructions
.syntax unified

// export the label asmMax expected by lab1math.h
.global asmMax

// .section marks a new section in assembly. .text identifies it as source code;
// .rodata marks it as read-only, setting it to go in FLASH, not SRAM
.section .text.rodata

/**
* void asmMax(float *array, uint32_t size, float *max, uint32_t *maxIndex);
*
* R0 = pointer to array
* R1 = size
* R2 = pointer to max
* R3 = pointer to maxIndex
*/

asmMax:
	PUSH			{R4, R5}			// saving R4, R5 for calling convention
	VLDR.f32		S0, [R0]			// FPU register S0 <- max = array[0]
	MOV				R4, #0				// R4 <- maxIndex = 0

loop:
	SUBS			R1, R1, #1			// R1 <- size = size - 1
	BLT				done				// loop finishes when R1 < 0
	ADD				R5, R0, R1, LSL #2	// R5 <- base address for array element (each is 4 bytes)
	VLDR.f32		S1, [R5]			// S1 <- new array element
	VCMP.f32		S0, S1				// compare (current max, new)
	VMRS			APSR_nzvc, FPSCR	// load FPU's PSR in order to branch using FPU conditions
	BGT				continue			// if max > new, go to next
	VMOV.f32		S0, S1				// otherwise S0 <- new max
	MOV				R4, R1				// update maxIndex

continue:
	B				loop				// next iteration

done:
	VSTR.f32		S0, [R2]			// store max in *max
	STR				R4, [R3]			// store maxIndex in *maxIndex
	POP				{R4, R5}			// restore saved variables
	BX				LR					// return
