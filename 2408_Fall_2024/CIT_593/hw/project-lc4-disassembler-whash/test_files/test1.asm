  .DATA           ; Lines below are Data Memory
  .ADDR x4020     ; Starting point of the Data Memory

global_array      ; The address x4020 is labeled as globa_array
  .FILL #6        ; x4020 = 6
  .FILL #5        ; x4021 = 5
  .FILL #8        ; x4022 = 8
  .FILL #10       ; x4023 = 10
  .FILL #-5       ; x4024 = -5

  .CODE           ; Lines below are Program Memory
  .ADDR x0000     ; Starting point of the Program Memory
  
	CONST R0, #5
	ADD R1, R0, #-1
  NOT R1, R2
  AND R3, R4, R3
  OR R3, R4, R3
  XOR R3, R4, R3
  AND R3, R4, #15

LOOP
	CMPI R0, #1
	BRnz END
	ADD R0, R0, #-1
	MUL R1, R1, R0
	JMP LOOP
END