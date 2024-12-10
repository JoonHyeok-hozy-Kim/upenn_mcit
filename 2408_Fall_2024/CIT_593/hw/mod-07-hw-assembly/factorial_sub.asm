;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  file name   : factorial_sub.asm                      ;
;  author      : Joon Hyeok Kim
;  description : LC4 Assembly program to compute the    ;
;                factorial of a number                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;;; pseudo-code of factorial_sub algorithm
;
; MAIN
;   A = 6;
;   B = sub_factorial (A);
;
;   int SUB_FACTORIAL (A) {
;     B = -1;
;     if (A < 0 || A > 7){  // Why 7?) We should return -1 if A does not pass the validation. 
;       return B;           // Thus, the output B should be in the signed integer format, and 7! < 2^{15}-1 < 8!. 
;     }                     // Therefore, A should be limited to 7.
;       
;     B = A;
;     while (A > 1){
;       A = A - 1;
;       B = B * A;
;     }
;     return B;
; 
; END
;       
;

;;; TO-DO: Implement the factorial_sub algorithm above using LC4 Assembly instructions

; Register allocation : R0(A)
CONST R0, #6          ; A = 6 (Edge cases tested with -1 and 9.)
JSR SUB_FACTORIAL     ; Call the SUB_FACTORIAL subroutine with the argument A. B = SUBROUINE(A).
JMP END               ; Jump over the SUB_FACTORIAL subroutine.

; SUB_FACTORIAL implementation
.FALIGN                   ; Allign the subroutine.
SUB_FACTORIAL             ; Argument : R0(A), RET: R1(B)
  CONST R1, #-1           ; Initialize by B = -1.
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
  RET                     ; End of the subroutine. Return the value B.

END                   ; End of the program.