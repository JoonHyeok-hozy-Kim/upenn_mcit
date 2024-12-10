;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  file name   : lc4_stdio.asm                          ;
;  author      : 
;  description : LC4 Assembly subroutines that call     ;
;                call the TRAPs in os.asm (the wrappers);
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Constants
  HEAP_MEMORY_TOP   .UCONST x4000

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;; WRAPPER SUBROUTINES FOLLOW ;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
.CODE
.ADDR x0010    ;; this code should be loaded after line 10
               ;; this is done to preserve "USER_START"
               ;; subroutine that calls "main()"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; TRAP_PUTC Wrapper ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.FALIGN
lc4_putc

	;; PROLOGUE ;;
        ; CIT 593 TODO: write prologue code here
        STR R7, R6, #-2	;; save return address
        STR R5, R6, #-3	;; save base pointer
        ADD R6, R6, #-3 ;; Update stack pointer
        ADD R5, R6, #0  ;; Update frame pointer
		
	;; FUNCTION BODY ;;
		; CIT 593 TODO: write code to get arguments to the trap from the stack
		;  and copy them to the register file for the TRAP call
        LDR R7, R5, #3  ;; Load the argument c into R7
        ADD R0, R7, #0  ;; Copy c into R0 to call TRAP x01
		
	TRAP x01        ; R0 must be set before TRAP_PUTC is called
	
	;; EPILOGUE ;; 
		; TRAP_PUTC has no return value, so nothing to copy back to stack
        CONST R7, #0    ;; Clear R7
        ;; No local variable in this function.
        ADD R6, R6, #3  ;; Decrease stack.
        STR R7, R6, #-1 ;; Update return value.
        LDR R5, R6, #-3 ;; Restore the base pointer (Frame pointer of main)
        LDR R7, R6, #-2 ;; Restore R7 for return

	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; TRAP_GETC Wrapper ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.FALIGN
lc4_getc

	;; PROLOGUE ;;
        ; CIT 593 TODO: write prologue code here
        STR R7, R6, #-2	;; save return address
        STR R5, R6, #-3	;; save base pointer
        ADD R6, R6, #-3 ;; Update stack pointer
        ADD R5, R6, #0  ;; Update frame pointer
		
	;; FUNCTION BODY ;;
	; CIT 593 TODO: TRAP_GETC doesn't require arguments!
        ;; No argument when calling TRAP_GETC
		
	TRAP x00        ; Call's TRAP_GETC 
                        ; R0 will contain ascii character from keyboard
                        ; you must copy this back to the stack

        ADD R7, R0, #0  ; Copy return value from TRAP_GETC
	
	;; EPILOGUE ;; 
		; TRAP_GETC has a return value, so make certain to copy it back to stack
        ;; No local variable in lc4_getc
        ADD R6, R6, #3  ;; Decrease stack.
        STR R7, R6, #-1 ;; Update return value.
        LDR R5, R6, #-3 ;; Restore the base pointer (Frame pointer of main)
        LDR R7, R6, #-2 ;; Restore R7 for return
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; TRAP_PUTS Wrapper ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.FALIGN
lc4_puts

	;; PROLOGUE ;;
        STR R7, R6, #-2	;; save return address
        STR R5, R6, #-3	;; save base pointer
        ADD R6, R6, #-3 ;; Update stack pointer
        ADD R5, R6, #0  ;; Update frame pointer
		
	;; FUNCTION BODY ;;
        ;; TRAP x03 - Input  : R0 = Address for first character
        ;;          - Output : None
    
        LDR R0, R5, #3  ; Load the argument into R0
		
	TRAP x03        ; Call's TRAP_PUTS 

        CONST R7, #0    ; Set return value into 0 and load into R7.
	
	;; EPILOGUE ;; 
        ;; No local variable in lc4_getc
        ADD R6, R6, #3  ;; Decrease stack.
        STR R7, R6, #-1 ;; Update return value.
        LDR R5, R6, #-3 ;; Restore the base pointer (Frame pointer of main)
        LDR R7, R6, #-2 ;; Restore R7 for return
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; TRAP_DRAW_REC Wrapper ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.FALIGN
lc4_draw_rec

	;; PROLOGUE ;;
        STR R7, R6, #-2	;; save return address
        STR R5, R6, #-3	;; save base pointer
        ADD R6, R6, #-3 ;; Update stack pointer
        ADD R5, R6, #0  ;; Update frame pointer
		
	;; FUNCTION BODY ;;
        ;;; TRAP x09 Inputs - R0 = “x coordinate” of upper-left corner of the rectangle.
        ;;;                 - R1 = “y coordinate” of upper-left corner of the rectangle.
        ;;;                 - R2 = length of the rectangle (in number of pixels).
        ;;;                 - R3 = width of the side of the rectangle (in number of pixels).
        ;;;                 - R4 = the color of the rectangle
        ;;;          Output - none
    
    LDR R7, R5, #3  ; Load the input argument into R7 : the address of the array
    LDR R0, R7, #0  ; R0 = stack[R7+0]
    LDR R1, R7, #1  ; R1 = stack[R7+1]
    LDR R2, R7, #2  ; R2 = stack[R7+2]
    LDR R3, R7, #3  ; R3 = stack[R7+3]

    LDR R4, R7, #4  ; R4 = stack[R7+4]
    LDR R7, R7, #5  ; R7 = stack[R7+4]
    SLL R7, R7, #8  ; R7 = R7 * 2^8
    ADD R4, R4, R7  ; R4 = R7(Upper 8bit) + R4(Lower 8bit)

    LC R7, HEAP_MEMORY_TOP  ; Load the Heap Memory Address of x4000 at R7.
    STR R6, R7, #0          ; Backup R6 at dmem[x4000]
		
	TRAP x09        ; Call's TRAP_PUTS 

    LC R7, HEAP_MEMORY_TOP  ; Load x4000 at R7.
    LDR R6, R7, #0          ; Restore R6.    

    CONST R7, #0    ; Set return value into 0 and load into R7.
	
	;; EPILOGUE ;; 
        ;; No local variable in lc4_getc
        ADD R6, R6, #3  ;; Decrease stack.
        STR R7, R6, #-1 ;; Update return value.
        LDR R5, R6, #-3 ;; Restore the base pointer (Frame pointer of main)
        LDR R7, R6, #-2 ;; Restore R7 for return
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; TRAP_RESET_VMEM Wrapper ;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.FALIGN
lc4_reset_vmem

	;; PROLOGUE ;;
        STR R7, R6, #-2	;; save return address
        STR R5, R6, #-3	;; save base pointer
        ADD R6, R6, #-3 ;; Update stack pointer
        ADD R5, R6, #0  ;; Update frame pointer

    TRAP x07        ; Call TRAP_RESET_VMEM

    CONST R7, #0    ; Set return value into 0 and load into R7.
	
	;; EPILOGUE ;; 
        ;; No local variable in lc4_getc
        ADD R6, R6, #3  ;; Decrease stack.
        STR R7, R6, #-1 ;; Update return value.
        LDR R5, R6, #-3 ;; Restore the base pointer (Frame pointer of main)
        LDR R7, R6, #-2 ;; Restore R7 for return
	RET
