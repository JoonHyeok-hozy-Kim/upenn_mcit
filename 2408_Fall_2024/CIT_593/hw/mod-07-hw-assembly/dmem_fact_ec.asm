;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  file name   : dmem_fact.asm                          ;
;  author      : Joon Hyeok Kim
;  description : LC4 Assembly program to compute the    ;
;                factorial of a number                  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;
;;; pseudo-code of dmem_fact algorithm
;
;   global_array = [6, 5, 8, 10, -5]
;   addr = address(global_array[0])
;
;   for (int i=0; i<5; i++){
;     SUB_FACTORIAL(addr + i);
;   }
;
;   int SUB_FACTORIAL(int a){
;     A = global_array[a];
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
;     global_array[a] = B;
;   }   
;

;;; TO-DO: Implement the dmem_fact algorithm above using LC4 Assembly instructions

  ; Data Section
  .DATA           ; Lines below are Data Memory
  .ADDR x4020     ; Starting point of the Data Memory

global_array      ; The address x4020 is labeled as globa_array
  .FILL #6        ; x4020 = 6
  .FILL #5        ; x4021 = 5
  .FILL #8        ; x4022 = 8
  .FILL #10       ; x4023 = 10
  .FILL #-5       ; x4024 = -5

  ; Start of the Code Section
  .CODE           ; Lines below are Program Memory
  .ADDR x0000     ; Starting point of the Program Memory

INIT
  LEA R2, global_array    ; Load the starting address of global_array to R2.
  ADD R3, R2, #5          ; Load R2 + len(global_array) [HARD CODED as 5 in this case] to R3. R3 = R2 + 5.

FOR_LOOP
  CMP R2, R3            ; Set NZP (R2-R3).
  BRzp END_FOR_LOOP     ; Test NZP (if positive (R2 >= R3)? goto END_FOR_LOOP)
  JSR SUB_FACTORIAL     ; Call SUB_FACTORIAL(R2). Then B(R1) = SUB_FACTORIAL(R2).
  ADD R2, R2, #1        ; Increment R2 by one.
  JMP FOR_LOOP          ; Jump to FOR_LOOP
  END_FOR_LOOP          ;
  JMP END               ; Jump to the end of the program

  ; SUB_FACTORIAL implementation
  .FALIGN                   ; Allign the subroutine.
SUB_FACTORIAL               ; Argument : R2(i)  
  LDR R0, R2, #0            ; Load the data at global_array[R2(i)] to R0. A = global_array[R2(i)].
  CONST R1, #-1             ; Initialize by B = -1.
  CMPI R0, #0               ; Set NZP(A-0). To check whether A is negative.
  BRn END_SUB_FACTORIAL     ; Test NZP (A negative? If yes, goto END_SUB_FACTORIAL.)
  CMPI R0, #7               ; Set NZP(A-7). Why?) 7! < 2^{15}-1 < 8! Refer to the Pseudo Code section for choosing 7 as the maximum valud of A.
  BRp END_SUB_FACTORIAL     ; Test NZP (A-7 positive? If yes, goto END_SUB_FACTORIAL.)

  ADD R1, R0, #0            ; Assign B = A.
  SUB_FACTORIAL_LOOP        ; Start a loop for the factorial calculation.
    CMPI R0, #1             ; Set NZP(A-1)
    BRnz END_SUB_FACTORIAL  ; Test NZP (A-1 neg or zero? If yes, goto END_SUB_FACTORIAL.)
    ADD R0, R0, #-1         ; A = A-1
    MUL R1, R1, R0          ; B = B * A
    JMP SUB_FACTORIAL_LOOP  ; Jump to LOOP
  END_SUB_FACTORIAL         ; Loop escaped point.
  STR R1, R2, #0            ; Overwrite the result. global_array[R2(i)] = B(R1).
  RET                       ; Jump back to R7

END                   ; End of the program.