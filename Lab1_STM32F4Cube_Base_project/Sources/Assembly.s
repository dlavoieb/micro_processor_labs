    AREA file123, CODE, READONLY

FIR_asm PROC
    EXPORT FIR_asm
    ; FIR Filter implementation in Assembly
    ; Input arguments:
    ;   R0 Pointer to start of data array
    ;   R1 Pointer to coefficient array
    ;   R2 Pointer to the output array
    ;   R3 Length of coefficient array
    ;   R12 Length of data array, same as number of output elements (from the stack)

    POP         {R12}                           ; Get R12 data counter from stack
    PUSH        {R5, LR}                        ; Store status of non-volatile registers
    LDR         R5, = 0                         ; R5 is loop counter, to all data elements
loop
    CMP         R12, R5                         ; Check if we reach loop count
    BEQ         exit

    VSUB.F32    S0, S0, S0                      ; Initialize total

    VLDM.F32    R0, {S1, S2, S3, S4, S5}        ; Load next data element
    VLDM.F32    R1, {S11, S12, S13, S14, S15}   ; Load next coefficient
    VFMA.F32    S0, S1, S11                     ; Multiply data and coeff, then accumulate on total
    VFMA.F32    S0, S2, S12                     ; Multiply data and coeff, then accumulate on total
    VFMA.F32    S0, S3, S13                     ; Multiply data and coeff, then accumulate on total
    VFMA.F32    S0, S4, S14                     ; Multiply data and coeff, then accumulate on total
    VFMA.F32    S0, S5, S15                     ; Multiply data and coeff, then accumulate on total
    VSTR.F32    S0, [R2]                        ; Write back floating point result to memory

    ADD         R0, R0, #4                      ; increment the values of r0 and r2 and r5
    ADD         R2, R2, #4
    ADD         R5, R5, #1

    B           loop                            ; branch to top
exit
    POP         {R5, LR}                        ; Restore status of non-volatile registers
    PUSH        {R12}
    BX          LR                              ; Return to calling function
    ENDP
    END