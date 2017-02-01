	AREA file123, CODE, READONLY

FIR_asm PROC
	EXPORT FIR_asm
	; FIR Filter implementation in Assembly
	; Input arguments:
	; 	R0 Pointer to start of data array
	;	R1 Pointer to coefficient array
	; 	R2 Pointer to the output array
	; 	R3 Length of coefficient array
	;	R4 Length of data array, same as number of output elements
    
	POP		{R12} 						; Get R12 data counter from stack
    PUSH    {R5, LR}
    LDR 	R5, = 0 					; R5 is loop counter, to all data elements
loop
	CMP 	R12, R5
	BEQ 	exit
	
	PUSH	{r0, r1, r2, r3, r12, r5}	; push current pointers and counts to the stack
	BL 		AVERAGE_asm 				; branch and link to average
	POP		{r0, r1, r2, r3, r12, r5} 	; on return pop from the stack the values or r0 - r4
	
	ADD 	R0, R0, #4					; increment the values of r0 and r2 and r5
	ADD 	R2, R2, #4
	ADD 	R5, R5, #1

	B 		loop						; branch to top
exit
    POP 	{R5, LR}
    PUSH    {R12}
	BX 		LR
	ENDP
		
AVERAGE_asm PROC
	EXPORT AVERAGE_asm
	; Compute the weighted average of two arrays
	; Input arguments:
	; 	R0 Pointer to data segment
	; 	R1 Pointer to coefficient array
	; 	R2 Pointer to output array
	;	R3 Length of coefficient array

	LDR			R12, = 0		; Initialize averaging_loop counter
	VSUB.F32 	S0, S0, S0		; Initialize total ; TODO: FIX THIS HACK INITIALISATION 
average_loop
	CMP			R12, R3 		; Compare loop index to coeffs length
	BEQ 		average_exit 	; If reached loop end branch out
	VLDR.F32 	S1, [R0] 		; Load next data element
	VLDR.F32	S2, [R1]		; Load next coefficient
	VFMA.F32	S0, S1, S2 		; Multiply data and coeff, then accumulate on total
	ADD 		R12, R12, #0x1 	; Increment index
	ADD 		R0, R0, #0x4 	; Shift data pointer to next element
	ADD 		R1, R1, #0x4 	; Shift data pointer to next element
	B 			average_loop	; Jump to beginning of loop
average_exit	
	VSTR.F32	S0, [R2] 		; Write back floating point result to memory
	BX 			LR				; Return from function call
	ENDP
		
	END