;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  file name   : user_string_ec.asm                       ;
;  author      : Joon Hyeok Kim                           ;
;  description : Get user input in 2 seconds.             ;
;                If the input is provided string them back to the ASCII display.
;	             Otherwise, return                        ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;

.DATA
.ADDR x4060
time_over_string
    .FILL x54       ; 'T'
    .FILL x49       ; 'I'
    .FILL x4D       ; 'M'
    .FILL x45       ; 'E'
    .FILL x20       ; SP
    .FILL x4F       ; 'O'
    .FILL x56       ; 'V'
    .FILL x45       ; 'E'
    .FILL x52       ; 'R'
    .FILL x21       ; '!'
    .FILL x00       ; NULL

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The following CODE will go into USER's Program Memory
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


.CODE
.ADDR x0000

    CONST R0, xD0     ; R0 = 2000 milliseconds = 2 seconds
    HICONST R0, x07   ; R0 = 2000 milliseconds = 2 seconds

    TRAP x05          ; Call TRAP_GETC_TIMER

    ;;; Test Script on Display
    ;;; Print char if the keyboard was properly input, otherwise print "TIME OVER!"
    CMPI R0, #0                 ; R0 == 0 (NULL)?
    ;BRz PRINT_TIME_OVER         ; (Disabled!) If yes, go to PRINT_TIME_OVER.
        TRAP x01                ; Display the input char.
        JMP END
    PRINT_TIME_OVER             ;
        LEA R0 time_over_string ; Prepare "TIME OVER!" 
        TRAP x03                ; Call TRAP_PUTS

END