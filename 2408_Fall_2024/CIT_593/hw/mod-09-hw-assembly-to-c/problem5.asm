		.DATA
L2_problem5 		.FILL #50
		.FILL #5
		.FILL #10
		.FILL #5
		.FILL #0
		.FILL #124
		.DATA
L3_problem5 		.FILL #10
		.FILL #10
		.FILL #50
		.FILL #40
		.FILL #224
		.FILL #7
		.DATA
L4_problem5 		.FILL #120
		.FILL #100
		.FILL #27
		.FILL #10
		.FILL #224
		.FILL #255
;;;;;;;;;;;;;;;;;;;;;;;;;;;;main;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		.CODE
		.FALIGN
main
	;; prologue
	STR R7, R6, #-2	;; save return address
	STR R5, R6, #-3	;; save base pointer
	ADD R6, R6, #-3
	ADD R5, R6, #0
	CONST R7, #18
	SUB R6, R6, R7	;; allocate stack space for local variables
	;; function body
	ADD R1, R5, #-6
	LEA R0, L2_problem5
;ASGNB
	ADD R6, R6, #-1
	STR R2, R6, #0
	ADD R6, R6, #-1
	STR R3, R6, #0
;blkloop!!!!
	AND R3, R3, #0
	ADD R3, R3, #6
L5
	LDR R2, R0, #0
	STR R2, R1, #0
	ADD R0, R0, #1
	ADD R1, R1, #1
	ADD R3, R3, #-1
BRnp L5
	LDR R3, R6, #0
	ADD R6, R6, #1
	LDR R2, R6, #0
	ADD R6, R6, #1
	ADD R1, R5, #-12
	LEA R0, L3_problem5
;ASGNB
	ADD R6, R6, #-1
	STR R2, R6, #0
	ADD R6, R6, #-1
	STR R3, R6, #0
;blkloop!!!!
	AND R3, R3, #0
	ADD R3, R3, #6
L6
	LDR R2, R0, #0
	STR R2, R1, #0
	ADD R0, R0, #1
	ADD R1, R1, #1
	ADD R3, R3, #-1
BRnp L6
	LDR R3, R6, #0
	ADD R6, R6, #1
	LDR R2, R6, #0
	ADD R6, R6, #1
	ADD R1, R5, #-16
	ADD R1, R1, #-2
	LEA R0, L4_problem5
;ASGNB
	ADD R6, R6, #-1
	STR R2, R6, #0
	ADD R6, R6, #-1
	STR R3, R6, #0
;blkloop!!!!
	AND R3, R3, #0
	ADD R3, R3, #6
L7
	LDR R2, R0, #0
	STR R2, R1, #0
	ADD R0, R0, #1
	ADD R1, R1, #1
	ADD R3, R3, #-1
BRnp L7
	LDR R3, R6, #0
	ADD R6, R6, #1
	LDR R2, R6, #0
	ADD R6, R6, #1
	ADD R7, R5, #-6
	ADD R6, R6, #-1
	STR R7, R6, #0
	JSR lc4_draw_rec
	LDR R7, R6, #-1	;; grab return value
	ADD R6, R6, #1	;; free space for arguments
	ADD R7, R5, #-12
	ADD R6, R6, #-1
	STR R7, R6, #0
	JSR lc4_draw_rec
	LDR R7, R6, #-1	;; grab return value
	ADD R6, R6, #1	;; free space for arguments
	ADD R7, R5, #-16
	ADD R7, R7, #-2
	ADD R6, R6, #-1
	STR R7, R6, #0
	JSR lc4_draw_rec
	LDR R7, R6, #-1	;; grab return value
	ADD R6, R6, #1	;; free space for arguments
	CONST R7, #0
L1_problem5
	;; epilogue
	ADD R6, R5, #0	;; pop locals off stack
	ADD R6, R6, #3	;; free space for return address, base pointer, and return value
	STR R7, R6, #-1	;; store return value
	LDR R5, R6, #-3	;; restore base pointer
	LDR R7, R6, #-2	;; restore return address
	RET

