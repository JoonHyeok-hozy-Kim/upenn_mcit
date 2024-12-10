;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  file name   : user_string2.asm                            ;
;  author      : 
;  description : read string from the keyboard,       ;
;	             then store them in the User Data ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The following CODE will go into USER's Program Memory
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.DATA
.ADDR x6000

result_string
    .FILL x4C       ; ASCII : L
    .FILL x65       ; ASCII : e
    .FILL x6E       ; ASCII : n
    .FILL x67       ; ASCII : g
    .FILL x74       ; ASCII : t
    .FILL x68       ; ASCII : h
    .FILL x20       ; ASCII : SP
    .FILL x3D       ; ASCII : =
    .FILL x20       ; ASCII : SP
    .FILL x00       ; NULL : End of the string.


.CODE
.ADDR x0000

    ; Store the string input.
    CONST R6, x20               ; Assign R6 = x2020
    HICONST R6, x20             ; Assign R6 = x2020
    ADD R0, R6, #0              ; Copy x2020 at R0
    TRAP x02                    ; Call TRAP_GETS.
    STR R1, R6, #-1             ; Backup R1 (length) at dmem[x201F]

    ; Print "Length = "
    LEA R0, result_string       ; Reset R0 into the address of result_string : "Length = "
    TRAP x03                    ; Call TRAP_PUTS.

    ; Generate ASCII char string for the integer length value.
    CONST R6, x20               ; Assign R6 = x2020
    HICONST R6, x20             ; Assign R6 = x2020
    LDR R0, R6, #-1             ; Load (length) from dmem[x201F] into R0 as an argument for the NUM_TO_STRING subroutine.
    JSR NUM_TO_STRING;          ; Call the NUM_TO_STRING subroutine to print length in ASCII.
    
    CONST R0, x0A               ; R0 = ASCII CR (Carriage Return)
    TRAP x01                    ; Call TRAP_PUTC. Change line for better visualization.

    ; Output the string on the display.
    CONST R0, x20               ; Assign R0 = x2020
    HICONST R0, x20             ; Assign R0 = x2020
    TRAP x03                    ; Call TRAP_PUTS.
    JMP END

.FALIGN
NUM_TO_STRING
    ; Convert integer value into array of chars in numbers and call TRAP_PUTS to print them.
    ; Argument : R0 (unsigned integer)
    ; Return Null

    CONST R1, x00   ; Set R1 = x7000 : Starting address of the length string
    HICONST R1, x70 ; Set R1 = x7000 : Starting address of the length string
    ADD R2, R1, #0  ; Copy R1 at R2 = x7000 : User Data Memory Pointer!
    CONST R3, #10   ; Put R3 = 10 (decimal power)
    CONST R5, x30   ; ASCII adjustment : '0' = x30

    ; Repeat storing mod result at dmem and divide R0 by 10.
    NUM_TO_STRING_WHILE
        MOD R4, R0, R3                  ; R5 = R0 % R3(10)
        ;;ADD R4, R5, #0                ; Copy R5 at R4 (Don't know why but cannot add #48 directly to R5).
        ADD R4, R4, R5                  ; ASCII adjustment by adding R5
        STR R4, R2, #0                  ; dmem[R2] = R4
        DIV R0, R0, R3                  ; R0 = R0 // 10
        ADD R2, R2, #1                  ; Increment R2.
        CMPIU R0, #0                     
        BRp NUM_TO_STRING_WHILE         ; R0 > 0? Go back to whlie loop.
    CONST R4, #0        ; R4 = x0000 : Final NULL for TRAP_PUTS.
    STR R4, R2, #0      ; Store dmem[R2] = x0000.

    ; Reverse the previously stored mod results.
    ADD R2, R2, #-1                 ; R2-- : Restore the address before x0000.
    ADD R6, R1, #0                  ; Copy R1=x7000 at R6.
    REVERSING_WHILE
        CMPU R6, R2                 ; R6 >= R2 ? 
        BRzp EXIT_REVERSING_WHILE   ; If yes, exit the while loop.
            LDR R3, R6, #0          ; Load R3 = dmem[R6]
            LDR R4, R2, #0          ; Load R4 = dmem[R2]
            STR R4, R6, #0          ; Store dmem[R6] = R4
            STR R3, R2, #0          ; Store dmem[R2] = R3
            ADD R6, R6, #1          ; R6++
            ADD R2, R2, #-1         ; R2--
            JMP REVERSING_WHILE
        EXIT_REVERSING_WHILE    

    ADD R0, R1, #0      ; Copy R1=x7000 at R0 as an argument for TRAP_PUTS.
    ADD R1, R1, #-1     ; R1 = x6FFF
    STR R7, R1, #0      ; Backup R7 at dmem[x6FFF]. Prevent from being overwritten by TRAP_PUTS.
    TRAP x03            ; Call TRAP_PUTS.
    CONST R1, xFF       ; R1 = x6FFF
    HICONST R1, x6F     ; R1 = x6FFF
    LDR R7, R1, #0      ; Restore the original R7.
    RET                 ; Return

END