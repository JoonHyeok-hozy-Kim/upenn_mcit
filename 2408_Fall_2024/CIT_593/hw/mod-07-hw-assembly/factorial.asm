;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  file name   : factorial.asm                          ;
;  author      : Joon Hyeok Kim
;  description : LC4 Assembly program to compute the    ;
;                factorial of a number                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;;; pseudo-code of factorial algorithm
;
; A = 5 ;  // example to do 5!
; B = A ;  // B=A! when while loop completes
;
; while (A > 1) {
; 	A = A - 1 ;
; 	B = B * A ;
; }
;

;;; TO-DO: Implement the factorial algorithm above using LC4 Assembly instructions

; Register allocation : A = 5, B = A
  CONST R0, #5    ; A = 5
  ADD R1, R0, #0  ; B = A+0

; While loop implementation
LOOP
  CMPI R0, #1     ; Set NZP(A-1)
  BRnz END        ; Test NZP (A-1 neg or zero? If yes, goto END.)
  ADD R0, R0, #-1 ; A = A-1
  MUL R1, R1, R0  ; B = B * A
  JMP LOOP        ; Jump to LOOP
END