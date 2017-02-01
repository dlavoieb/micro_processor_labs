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
	LDR		R5, = 0		; Initialize loop counter
	VSUB.F32 S0, S0, S0	; Initialize total ; TODO: FIX THIS HACK INITIALISATION 
loop
	CMP		R5, R3 		; Compare loop index to coeffs length
	BEQ 	exit 		; If reached loop end branch out
	VLDR.F32 	S1, [R0] 	; Load next data element
	VLDR.F32	S2, [R1]	; Load next coefficient
	VFMA.F32	S0, S1, S2 	; Multiply data and coeff, then accumulate on total
	ADD R5, R5, #0x1 	; Increment index
	ADD R0, R0, #0x4 	; Shift data pointer to next element
	ADD R1, R1, #0x4 	; Shift data pointer to next element
	B 			loop 		; Jump to beginning of loop
exit
	
	VSTR.F32	S0, [R2] 	; Write back floating point result to memory
	BX LR ; Return from function call
	ENDP
		
	END