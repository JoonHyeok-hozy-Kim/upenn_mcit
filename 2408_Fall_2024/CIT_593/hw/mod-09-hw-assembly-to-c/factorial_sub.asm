;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  file name   : factorial_sub.asm                      ;
;  author      : 
;  description : LC4 Assembly subroutine to compute the ;
;                factorial of a number                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;;; pseudo-code of factorial algorithm
;- Input : a
;
; b = a ;  // b = a
;
; while (a > 1) {
; 	a = a - 1 ;
; 	b = b * a ;
; }
;

;; CIT 593 TO-DO:
;; 1) Open up the codio assignment where you created the factorial subroutine (in a separate browswer window)
;; 2) In that window, open up your working factorial_sub.asm file:
;;    -select everything in the file, and "copy" this content (Conrol-C) 
;; 3) Return to the current codio assignment, paste the content into this factorial_sub.asm 
;;    -now you can use the factorial_sub.asm from your last HW in this HW
;; 4) Save the updated factorial_sub.asm file

.FALIGN
SUB_FACTORIAL              
    ;;; prologue
	STR R7, R6, #-2	        ; Save return address.
	STR R5, R6, #-3	        ; Save frame pointer.
	ADD R6, R6, #-3         ; Update stack pointer.
	ADD R5, R6, #0          ; Update frame pointer.
	ADD R6, R6, #-1	        ; Allocate stack space for a local variable b.
	LDR R0, R5, #3          ; Load the argument a at R0.

    ;;; body
    CONST R1, #-1           ; Initialize by b = -1.
    CMPI R0, #0             ; Set NZP(A-0). To check whether A is negative.
    BRn END_SUB_FACTORIAL   ; Test NZP (A negative? If yes, goto END_SUB_FACTORIAL.)
    CMPI R0, #7             ; Set NZP(A-7). Why?) 7! < 2^{15}-1 < 8! Refer to the Pseudo Code section for choosing 7 as the maximum valud of A.
    BRp END_SUB_FACTORIAL   ; Test NZP (A-7 positive? If yes, goto END_SUB_FACTORIAL.)

    ADD R1, R0, #0          ; Assign B = A.
    LOOP                    ; Start a loop for the factorial calculation.
        CMPI R0, #1             ; Set NZP(A-1)
        BRnz END_SUB_FACTORIAL  ; Test NZP (A-1 neg or zero? If yes, goto END_SUB_FACTORIAL.)
        ADD R0, R0, #-1         ; A = A-1
        MUL R1, R1, R0          ; B = B * A
        JMP LOOP                ; Jump to LOOP
    END_SUB_FACTORIAL       ; Loop escaped point.


    ;;; epilogue
    ADD R6, R5, #0          ; Pop local variable.
    ADD R6, R6, #3          ; Decrease stack.
    STR R1, R6, #-1         ; Update return value.
    LDR R5, R6, #-3         ; Restore frame pointer.
    LDR R7, R6, #-2         ; Restore R7 for RET.
    RET                     ; Return to Caller!

