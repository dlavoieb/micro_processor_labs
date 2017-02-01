	AREA file123, CODE, READONLY

FIR_asm PROC
	EXPORT FIR_asm
	; FIR Filter implementation in Assembly
	; Input arguments:
	; 	R0 Pointer to start of data array
	;	R1 Pointer to coefficient array
	;	R2 Length of data array
	; 	R3 Length of coefficient array
	
	ADD 	R0, R0, # 0x0 ; nop
	LDR     R0, = AVERAGE_asm
	;BX      R0
	ENDP
		
AVERAGE_asm PROC
	EXPORT AVERAGE_asm
	; Compute the weighted average of two arrays
	; Input arguments:
	; 	R0 Pointer to data segment
	; 	R1 Pointer to coefficient array
	; 	R2 Pointer to output array
	;	R3 Length of coefficient array
	;	R4 Length of the output array
	
	; Outer loop: Goes through all the data segments.
	LDR		R6, = 0		; Initialize the outer loop counter.
	
	; Set R10 such that it is 1 less than the number of words in coefficient array
	; in bytes, hence the multiplication. This will allow us to shift back our data
	; segment at the end of the inner loop.
	LDR 	R10, = 4 	; Initialize R10
	
	; Multiply the number of bytes per word by the 
	; length of the coeff array and subtract from 1
	; to give appropriate shift
	MLS		R10, R10, R3, = 1	

outer_loop
	;initialize the counter of the outer loop .. done
	;set the ammount that we have to shift back by .. done

	;Outer loop
		;exit if we have exceeded the outer loop counter. 

		;Take the average starting at the position based on the outer loop
		;counter as an offset.
		
		;Set the current output register to the result

		;Increment the address of the output register	

		;Increment the outer loop counter.

		;Set the Data pointer register to the starting value plus offset

		;Reset to coefficient pointer back to the start of the array
	
	LDR		R5, = 0		; Initialize averaging_loop counter
	VSUB.F32 S0, S0, S0	; Initialize total ; TODO: FIX THIS HACK INITIALISATION 
averaging_loop
	CMP		R5, R3 		; Compare loop index to coeffs length
	BEQ 	exit_averaging_loop 		; If reached loop end branch out
	VLDR.F32 	S1, [R0] 	; Load next data element
	VLDR.F32	S2, [R1]	; Load next coefficient
	VFMA.F32	S0, S1, S2 	; Multiply data and coeff, then accumulate on total
	ADD R5, R5, #0x1 	; Increment index
	ADD R0, R0, #0x4 	; Shift data pointer to next element
	ADD R1, R1, #0x4 	; Shift data pointer to next element
	B 			averaging_loop 		; Jump to beginning of loop
exit_averaging_loop


exit_outer_loop
	
	VSTR.F32	S0, [R2] 	; Write back floating point result to memory
	BX LR ; Return from function call
	ENDP
		
	END