;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  file name   : user_string.asm                            ;
;  author      : 
;  description : read characters from the memory,       ;
;	             then string them back to the ASCII display ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The following CODE will go into USER's Program Memory
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.DATA
.ADDR x4000

love_systems
    .FILL x49       ; ASCII : 'I'
    .FILL x20       ; ASCII : SP
    .FILL x6C       ; ASCII : 'l'
    .FILL x6F       ; ASCII : 'o'
    .FILL x76       ; ASCII : 'v'
    .FILL x65       ; ASCII : 'e'
    .FILL x20       ; ASCII : SP
    .FILL x43       ; ASCII : 'C'
    .FILL x49       ; ASCII : 'I'
    .FILL x54       ; ASCII : 'T'
    .FILL x20       ; ASCII : SP
    .FILL x35       ; ASCII : '5'
    .FILL x39       ; ASCII : '9'
    .FILL x33       ; ASCII : '3'
    .FILL x00       ; NULL : End of the string.


.CODE
.ADDR x0000

    LEA R0, love_systems    ; Load love_systems address at R0 as an argument for TRAP_PUTS.
    TRAP x03                ; Call TRAP_PUTS.

END