/*
 * asmstd.s
 *
 *  Created on: Sep 17, 2022
 *      Author: Jimmy Sheng
 */

.syntax unified

.global asmStd

.section .text.rodata

/*
 void asmStd(const float *list, uint32_t list_length, float *result);

 R0 = ptr list
 R1 = length
 R2 = ptr result
 */

asmStd:
	PUSH			{R4-R5}				// save state
	MOV				R4, #0				// R4 <- index = 0
	MOV				R5, R0				// R5 <- list addr + offset
	VMOV			S0, R4				// S0 <- sum = R4 = 0 (because VMOV doesn't accept #imm = #0)

sum_loop:
	CMP				R4, R1
	BGE				end_sum_loop		// loop ends when index >= length
	VLDR.32			S1, [R5]			// S1 <- list[i]
	VADD.F32		S0, S0, S1
	// increment index and addr
	ADD				R4, R4, #1
	ADD				R5, R5, #4
	B				sum_loop

end_sum_loop:
	VMOV			S1, R1				// S1 <- length (need to be converted)
	VCVT.F32.U32	S1, S1				// uint32_t to single precision float
	VDIV.F32		S2, S0, S1			// S2 <- mean = sum / length
	VMOV			S3, #1				// S3 <- temporary value to perform length - 1
	VSUB.F32		S1, S1, S3			// S1 <- length - 1
	// prepare to loop
	MOV				R4, #0				// R4 <- index = 0
	MOV				R5, R0				// R5 <- list addr + offset
	VMOV			S0, R4				// S0 <- sum = R4 = 0

variance_loop:
	CMP				R4, R1
	BGE				end_variance_loop	// loop ends when index >= length
	VLDR.32			S3, [R5]			// S3 <- list[i]
	VSUB.F32		S3, S3, S2			// S3 <- list[i] - mean
	VMLA.F32		S0, S3, S3			// multiply and accumulate (would VMUL + VADD be faster?)
	// increment index and addr
	ADD				R4, R4, #1
	ADD				R5, R5, #4
	B				variance_loop

end_variance_loop:
	VDIV.F32		S0, S0, S1			// S0 <- variance = sum / (length - 1)
	VSQRT.F32		S0, S0				// S0 <- standard deviation = sqrt(variance)
	VSTR.32			S0, [R2]			// *result = S0
	POP				{R4-R5}
	BX				LR

