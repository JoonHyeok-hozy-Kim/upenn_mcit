		.DATA
L2_problem6 		.FILL #60
		.FILL #100
		.FILL #20
		.FILL #5
		.FILL #224
		.FILL #255
		.DATA
L3_problem6 		.FILL #60
		.FILL #100
		.FILL #20
		.FILL #5
		.FILL #0
		.FILL #0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;main;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		.CODE
		.FALIGN
main
	;; prologue
	STR R7, R6, #-2	;; save return address
	STR R5, R6, #-3	;; save base pointer
	ADD R6, R6, #-3
	ADD R5, R6, #0
	ADD R6, R6, #-15	;; allocate stack space for local variables
	;; function body
	ADD R1, R5, #-7
	LEA R0, L2_problem6
;ASGNB
	ADD R6, R6, #-1
	STR R2, R6, #0
	ADD R6, R6, #-1
	STR R3, R6, #0
;blkloop!!!!
	AND R3, R3, #0
	ADD R3, R3, #6
L15
	LDR R2, R0, #0
	STR R2, R1, #0
	ADD R0, R0, #1
	ADD R1, R1, #1
	ADD R3, R3, #-1
BRnp L15
	LDR R3, R6, #0
	ADD R6, R6, #1
	LDR R2, R6, #0
	ADD R6, R6, #1
	ADD R1, R5, #-13
	LEA R0, L3_problem6
;ASGNB
	ADD R6, R6, #-1
	STR R2, R6, #0
	ADD R6, R6, #-1
	STR R3, R6, #0
;blkloop!!!!
	AND R3, R3, #0
	ADD R3, R3, #6
L16
	LDR R2, R0, #0
	STR R2, R1, #0
	ADD R0, R0, #1
	ADD R1, R1, #1
	ADD R3, R3, #-1
BRnp L16
	LDR R3, R6, #0
	ADD R6, R6, #1
	LDR R2, R6, #0
	ADD R6, R6, #1
	CONST R7, #0
	STR R7, R5, #-1
	ADD R7, R5, #-7
	ADD R6, R6, #-1
	STR R7, R6, #0
	JSR lc4_draw_rec
	LDR R7, R6, #-1	;; grab return value
	ADD R6, R6, #1	;; free space for arguments
	JMP L5_problem6
L4_problem6
	JSR lc4_getc
	LDR R7, R6, #-1	;; grab return value
	ADD R6, R6, #0	;; free space for arguments
	STR R7, R5, #-1
	LDR R7, R5, #-1
	CONST R3, #27
	CMP R7, R3
	BRnp L7_problem6
	JMP L6_problem6
L7_problem6
	LDR R7, R5, #-1
	STR R7, R5, #-14
	CONST R3, #106
	CMP R7, R3
	BRz L11_problem6
	CONST R7, #74
	LDR R3, R5, #-14
	CMP R3, R7
	BRnp L9_problem6
L11_problem6
	LDR R7, R5, #-7
	ADD R7, R7, #-10
	STR R7, R5, #-7
	ADD R7, R5, #-13
	ADD R6, R6, #-1
	STR R7, R6, #0
	JSR lc4_draw_rec
	LDR R7, R6, #-1	;; grab return value
	ADD R6, R6, #1	;; free space for arguments
	LDR R7, R5, #-13
	ADD R7, R7, #-10
	STR R7, R5, #-13
	JMP L10_problem6
L9_problem6
	LDR R7, R5, #-1
	STR R7, R5, #-15
	CONST R3, #107
	CMP R7, R3
	BRz L14_problem6
	CONST R7, #75
	LDR R3, R5, #-15
	CMP R3, R7
	BRnp L5_problem6
L14_problem6
	LDR R7, R5, #-7
	ADD R7, R7, #10
	STR R7, R5, #-7
	ADD R7, R5, #-13
	ADD R6, R6, #-1
	STR R7, R6, #0
	JSR lc4_draw_rec
	LDR R7, R6, #-1	;; grab return value
	ADD R6, R6, #1	;; free space for arguments
	LDR R7, R5, #-13
	ADD R7, R7, #10
	STR R7, R5, #-13
L13_problem6
L10_problem6
	ADD R7, R5, #-7
	ADD R6, R6, #-1
	STR R7, R6, #0
	JSR lc4_draw_rec
	LDR R7, R6, #-1	;; grab return value
	ADD R6, R6, #1	;; free space for arguments
L5_problem6
	JMP L4_problem6
L6_problem6
	CONST R7, #0
L1_problem6
	;; epilogue
	ADD R6, R5, #0	;; pop locals off stack
	ADD R6, R6, #3	;; free space for return address, base pointer, and return value
	STR R7, R6, #-1	;; store return value
	LDR R5, R6, #-3	;; restore base pointer
	LDR R7, R6, #-2	;; restore return address
	RET

