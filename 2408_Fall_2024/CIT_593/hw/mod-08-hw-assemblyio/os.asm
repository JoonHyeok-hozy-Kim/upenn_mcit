;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  file name   : os.asm                                 ;
;  author      : 
;  description : LC4 Assembly program to serve as an OS ;
;                TRAPS will be implemented in this file ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;   OS - TRAP VECTOR TABLE   ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.OS
.CODE
.ADDR x8000
  ; TRAP vector table
  JMP TRAP_GETC           ; x00
  JMP TRAP_PUTC           ; x01
  JMP TRAP_GETS           ; x02
  JMP TRAP_PUTS           ; x03
  JMP TRAP_TIMER          ; x04
  JMP TRAP_GETC_TIMER     ; x05
  JMP TRAP_DRAW_PIXEL     ; x06
  JMP TRAP_RESET_VMEM	    ; x07
  JMP TRAP_BLT_VMEM	      ; x08

  ;
  ; CIT 593 - TO DO - add a vector for TRAP_DRAW_RECT
  ; place this vector at x8009
  ;
  JMP TRAP_DRAW_RECT	  ; x09
  
  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;   OS - MEMORY ADDRESSES & CONSTANTS   ;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

  ;; these handy alias' will be used in the TRAPs that follow
  USER_CODE_ADDR .UCONST x0000	; start of USER code
  OS_CODE_ADDR 	 .UCONST x8000	; start of OS code

  OS_GLOBALS_ADDR .UCONST xA000	; start of OS global mem
  OS_STACK_ADDR   .UCONST xBFFF	; start of OS stack mem

  OS_KBSR_ADDR .UCONST xFE00  	; alias for keyboard status reg
  OS_KBDR_ADDR .UCONST xFE02  	; alias for keyboard data reg

  OS_ADSR_ADDR .UCONST xFE04  	; alias for display status register
  OS_ADDR_ADDR .UCONST xFE06  	; alias for display data register

  OS_TSR_ADDR .UCONST xFE08 	; alias for timer status register
  OS_TIR_ADDR .UCONST xFE0A 	; alias for timer interval register

  OS_VDCR_ADDR	.UCONST xFE0C	; video display control register
  OS_MCR_ADDR	.UCONST xFFEE	; machine control register
  OS_VIDEO_NUM_COLS .UCONST #128
  OS_VIDEO_NUM_ROWS .UCONST #124

  HOZY_DRAW_RECT_ARRAY .UCONST xB000  ; offset #0 = “x coordinate” of upper-left corner of the rectangle.
                                      ; offset #1 = “y coordinate” of upper-left corner of the rectangle.
                                      ; offset #2 = length of the rectangle (in number of pixels).
                                      ; offset #3 = width of the side of the rectangle (in number of pixels).
                                      ; offset #4 = the color of the rectangle
                                      ; offset #5 = R7 (Return address)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;; OS DATA MEMORY RESERVATIONS ;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.DATA
.ADDR xA000
OS_GLOBALS_MEM	.BLKW x1000
;;;  LFSR value used by lfsr code
LFSR .FILL 0x0001

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;; OS VIDEO MEMORY RESERVATION ;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.DATA
.ADDR xC000
OS_VIDEO_MEM .BLKW x3E00

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;   OS & TRAP IMPLEMENTATIONS BEGIN HERE   ;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.CODE
.ADDR x8200
.FALIGN
  ;; first job of OS is to return PennSim to x0000 & downgrade privledge
  CONST R7, #0   ; R7 = 0
  RTI            ; PC = R7 ; PSR[15]=0


;;;;;;;;;;;;;;;;;;;;;;;;;;;   TRAP_GETC   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Function: Get a single character from keyboard
;;; Inputs           - none
;;; Outputs          - R0 = ASCII character from ASCII keyboard

.CODE
TRAP_GETC
    LC R0, OS_KBSR_ADDR  ; R0 = address of keyboard status reg
    LDR R0, R0, #0       ; R0 = value of keyboard status reg
    BRzp TRAP_GETC       ; if R0[15]=1, data is waiting!
                             ; else, loop and check again...

    ; reaching here, means data is waiting in keyboard data reg

    LC R0, OS_KBDR_ADDR  ; R0 = address of keyboard data reg
    LDR R0, R0, #0       ; R0 = value of keyboard data reg
    RTI                  ; PC = R7 ; PSR[15]=0


;;;;;;;;;;;;;;;;;;;;;;;;;;;   TRAP_PUTC   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Function: Put a single character out to ASCII display
;;; Inputs           - R0 = ASCII character to write to ASCII display
;;; Outputs          - none

.CODE
TRAP_PUTC
  LC R1, OS_ADSR_ADDR 	; R1 = address of display status reg
  LDR R1, R1, #0    	  ; R1 = value of display status reg
  BRzp TRAP_PUTC    	  ; if R1[15]=1, display is ready to write!
		    	              ; else, loop and check again...

  ; reaching here, means console is ready to display next char

  LC R1, OS_ADDR_ADDR 	; R1 = address of display data reg
  STR R0, R1, #0    	; R1 = value of keyboard data reg (R0)
  RTI			; PC = R7 ; PSR[15]=0


;;;;;;;;;;;;;;;;;;;;;;;;;;;   TRAP_GETS   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Function: Get a string of characters from the ASCII keyboard
;;; Inputs           - R0 = Address to place characters from keyboard
;;; Outputs          - R1 = Lenght of the string without the NULL

.CODE
TRAP_GETS

  ;;
  ;; CIT 593 TO DO: complete this trap
  ;;
  CONST R1, x00       ; Initialize output R1 into 0.
  HICONST R1, x00     ; Initialize output R1 into 0.

  CONST R3, x00       ; Set R3 = x2000 for validation
  HICONST R3, x20     ; Set R3 = x2000 for validation
  CMP R0, R3          ; R0 < x2000 ?
  BRn END_TRAP_GETS   ; Invalid memory address! Return.

  CONST R4, xFF       ; Set R4 = x7FFF for validation
  HICONST R4, x7F     ; Set R4 = x7FFF for validation

  TRAP_GETS_WHILE
    ;; Validation for the case that the address reached x7FFF!
    CMPU R0, R4               ; R0 == x7FFF ?
    BRz TRAP_GETS_WRITE_NULL  ; If yes, it reached the end of the User Data Memory. Write null and return.

    ;; Validation for the invalid address R0 > x7FFF
    CMPU R0, R4               ; R0 > x7FFF ?
    BRp END_TRAP_GETS         ; Invalid memory address! Return.

    TRAP_GETS_GETC_COPY
      LC R2, OS_KBSR_ADDR       ; R2 = address of keyboard status reg
      LDR R2, R2, #0            ; R2 = value of keyboard status reg
      BRzp TRAP_GETS_GETC_COPY  ; if R2[15]=1, data is waiting!

    LC R2, OS_KBDR_ADDR   ; R0 = address of keyboard data reg
    LDR R2, R2, #0        ; R0 = value of keyboard data register

    CMPI R2, x0A              ; R2 == x0A (enter = LF)?
    BRz TRAP_GETS_WRITE_NULL  ; If yes, write null and return.
      STR R2, R0, #0          ; Store the char: dmem[R0] = R2.
      ADD R0, R0, #1          ; Increment the memory address.
      ADD R1, R1, #1          ; Increment the output (length of the string).
      JMP TRAP_GETS_WHILE     ; Jump back to the beginning of the while loop.
  
  TRAP_GETS_WRITE_NULL
  CONST R2, x00     ; Set R2 = x0000  
  HICONST R2, x00   ; Set R2 = x0000
  STR R2, R0, #0    ; Store NULL at the end of the array.
  END_TRAP_GETS

  RTI               ; PC = R7 ; PSR[15]=0

;;;;;;;;;;;;;;;;;;;;;;;;;;;   TRAP_PUTS   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Function: Put a string of characters out to ASCII display
;;; Inputs           - R0 = Address for first character
;;; Outputs          - none

.CODE
TRAP_PUTS

  ;;
  ;; CIT 593 TO DO: complete this trap
  ;;

  ;;; Validation logic for x2000 <= R0 <= x7FFF
  CONST R1, x00               ; Set R1 = x2000
  HICONST R1, x20             ; Set R1 = x2000
  CMPU R0, R1                 ; R0 < R1?
  BRn EXIT_TRAP_PUTS_WHILE    ; If yes, return!
  CONST R1, xFF               ; Set R1 = x7FFF
  HICONST R1, x7F             ; Set R1 = x7FFF
  CMPU R0, R1                 ; R0 > R1?
  BRp EXIT_TRAP_PUTS_WHILE    ; If yes, return!

  TRAP_PUTS_WHILE
    LDR R2, R0, #0            ; Load dmem[R0] at R2.
    BRz EXIT_TRAP_PUTS_WHILE  ; R2 = x0000 ? Exit while loop.

      ; TRAP_PUTC logic copied!
      TRAP_PUTS_PUTC_COPY
        LC R3, OS_ADSR_ADDR 	    ; R3 = address of display status reg
        LDR R3, R3, #0    	      ; R3 = value of display status reg
        BRzp TRAP_PUTS_PUTC_COPY  ; if R3[15]=1, display is ready to write!
		    	                        ; else, loop and check again...
      LC R3, OS_ADDR_ADDR 	      ; R3 = address of display data reg
      STR R2, R3, #0    	        ; R3 = value of keyboard data reg (R2)

      ADD R0, R0, #1          ; R0 = curr_address++
      JMP TRAP_PUTS_WHILE     ; Jump back to while loop.
    EXIT_TRAP_PUTS_WHILE

  RTI                         ; PC = R7 ; PSR[15]=0


;;;;;;;;;;;;;;;;;;;;;;;;;   TRAP_TIMER   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Function:
;;; Inputs           - R0 = time to wait in milliseconds
;;; Outputs          - none

.CODE
TRAP_TIMER
  LC R1, OS_TIR_ADDR 	; R1 = address of timer interval reg
  STR R0, R1, #0    	; Store R0 in timer interval register

COUNT
  LC R1, OS_TSR_ADDR  	; Save timer status register in R1
  LDR R1, R1, #0    	; Load the contents of TSR in R1
  BRzp COUNT    	; If R1[15]=1, timer has gone off!

  ; reaching this line means we've finished counting R0

  RTI       		; PC = R7 ; PSR[15]=0



;;;;;;;;;;;;;;;;;;;;;;;   TRAP_GETC_TIMER   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Function: Get a single character from keyboard
;;; Inputs           - R0 = time to wait
;;; Outputs          - R0 = ASCII character from keyboard (or NULL)

.CODE
TRAP_GETC_TIMER

  ;;
  ;; CIT 593 TO DO: complete this trap
  ;;

  LC R1, OS_TIR_ADDR 	; R1 = address of timer interval reg
  STR R0, R1, #0    	; Store R0 in timer interval register

  CONST R0, #0        ; Initialize the output R0 value into 0.

TRAP_GETC_TIMER_LOOP
  LC R1, OS_TSR_ADDR  	      ; Save timer status register in R1
  LDR R1, R1, #0    	        ; Load the contents of TSR in R1
  BRn EXIT_BY_TIMEOUT         ; If R1[15]=1, timer has gone off, i.e. "time out."

  LC R1, OS_KBSR_ADDR         ; R1 = address of keyboard status reg
  LDR R1, R1, #0              ; R1 = value of keyboard status reg
  BRzp TRAP_GETC_TIMER_LOOP   ; if R1[15]=1, data is waiting! Exit loop
                              ; else, loop and check again...

  LC R0, OS_KBDR_ADDR         ; R0 = address of keyboard data reg
  LDR R0, R0, #0              ; R0 = value of keyboard data reg

  EXIT_BY_TIMEOUT             ; Escaped the loop due to time out.

  RTI                         ; PC = R7 ; PSR[15]=0


;;;;;;;;;;;;;;;;;;;;;;;;;   TRAP_DRAW_PIXEL   ;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Function: Draw point on video display
;;; Inputs           - R0 = row to draw on (y)
;;;                  - R1 = column to draw on (x)
;;;                  - R2 = color to draw with
;;; Outputs          - none

.CODE
TRAP_DRAW_PIXEL
  LEA R3, OS_VIDEO_MEM       ; R3=start address of video memory
  LC  R4, OS_VIDEO_NUM_COLS  ; R4=number of columns

  CMPIU R1, #0    	         ; Checks if x coord from input is > 0
  BRn END_PIXEL
  CMPIU R1, #127    	     ; Checks if x coord from input is < 127
  BRp END_PIXEL
  CMPIU R0, #0    	         ; Checks if y coord from input is > 0
  BRn END_PIXEL
  CMPIU R0, #123    	     ; Checks if y coord from input is < 123
  BRp END_PIXEL

  MUL R4, R0, R4      	     ; R4= (row * NUM_COLS)
  ADD R4, R4, R1      	     ; R4= (row * NUM_COLS) + col
  ADD R4, R4, R3      	     ; Add the offset to the start of video memory
  STR R2, R4, #0      	     ; Fill in the pixel with color from user (R2)

END_PIXEL
  RTI       		         ; PC = R7 ; PSR[15]=0
  


;;;;;;;;;;;;;;;;;;;;;;;;;;;;; TRAP_RESET_VMEM ;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; In double-buffered video mode, resets the video display
;;; CIT 593 students do not need to modify this trap, it's for future HWs
;;; Inputs - none
;;; Outputs - none
.CODE	
TRAP_RESET_VMEM
	LC R4, OS_VDCR_ADDR
	CONST R5, #1
	STR R5, R4, #0
	RTI

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; TRAP_BLT_VMEM ;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; TRAP_BLT_VMEM - In double-buffered video mode, copies the contents
;;; of video memory to the video display.
;;; CIT 593 students do not need to modify this trap, it's for future HWs
;;; Inputs - none
;;; Outputs - none
.CODE
TRAP_BLT_VMEM
	LC R4, OS_VDCR_ADDR
	CONST R5, #2
	STR R5, R4, #0
	RTI


;; CIT 593 TO DO: Add TRAP: TRAP_DRAW_REC
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; TRAP_DRAW_RECT ;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; TRAP_DRAW_RECT - Draw a solid (filled in) rectangle whose location 
;;; and dimensions will be set by the user.
;;; Inputs - R0 = “x coordinate” of upper-left corner of the rectangle.
;;;        - R1 = “y coordinate” of upper-left corner of the rectangle.
;;;        - R2 = length of the rectangle (in number of pixels).
;;;        - R3 = width of the side of the rectangle (in number of pixels).
;;;        - R4 = the color of the rectangle
;;; Outputs - none
;;;
;;; Use data memory xB000 ~ xB005 as storage of argument values.
;;; Refer to "OS - MEMORY ADDRESSES & CONSTANTS" on top.
;;; HOZY_DRAW_RECT_ARRAY .UCONST xB000  ; offset #0 = “x coordinate” of upper-left corner of the rectangle.
;;;                                     ; offset #1 = “y coordinate” of upper-left corner of the rectangle.
;;;                                     ; offset #2 = length of the rectangle (in number of pixels).
;;;                                     ; offset #3 = width of the side of the rectangle (in number of pixels).
;;;                                     ; offset #4 = the color of the rectangle
;;;                                     ; offset #5 = R7 (Return address)
;;;                                     ; offset #6 = Pixel Pointer

.CODE
TRAP_DRAW_RECT
  LC R6, HOZY_DRAW_RECT_ARRAY ; Memory address for storing the argument values.
  STR R0, R6, #0              ; Store R0 at dmem[xB000] : “x coordinate” of upper-left corner of the rectangle.
  STR R1, R6, #1              ; Store R1 at dmem[xB001] : “y coordinate” of upper-left corner of the rectangle.
  STR R2, R6, #2              ; Store R2 at dmem[xB002] : length of the rectangle (in number of pixels).
  STR R3, R6, #3              ; Store R3 at dmem[xB003] : width of the side of the rectangle (in number of pixels).
  STR R4, R6, #4              ; Store R4 at dmem[xB004] : the color of the rectangle
  STR R7, R6, #5              ; Store R7 at dmem[xB005] : R7 (Return address)

  LEA R3, OS_VIDEO_MEM       ; R3=start address of video memory
  LC  R4, OS_VIDEO_NUM_ROWS  ; R4=number of rows    = #124
  LC  R5, OS_VIDEO_NUM_COLS  ; R5=number of columns = #128

  ; Boundary Checking
  ; - The trap should check to see if the length/width are valid from the starting location of the box
  ;   - The trap should check to see if the length/width are valid from at the starting location of the box
  ;   - If invalid, use (0,0) instead, but keep the width/length
  ;   - If the box would go outside of video memory, correct the rectangle and make it “wrap around” the display


  ;;; 0) Length/Width validation for wrapping
  ;;; 0-1) Length validation
  LDR R1, R6, #2                ; Load length into R1
  CMPI R1, #0                   ; length <= 0 ?
  BRnz EXIT_DRAW_SQUARE         ; If yes, no drawing. Go to EXIT_DRAW_SQUARE.
  CMP R1, R5                    ; length > NUM_COLS ?
  BRp EXIT_DRAW_SQUARE          ; If yes, no drawing. Go to EXIT_DRAW_SQUARE.
  ;;; 0-2) Width validation
  LDR R1, R6, #3                ; Load width into R1
  CMPI R1, #0                   ; length <= 0 ?
  BRnz EXIT_DRAW_SQUARE         ; If yes, no drawing. Go to EXIT_DRAW_SQUARE.
  CMP R1, R4                    ; width > NUM_ROWS ?
  BRp EXIT_DRAW_SQUARE          ; If yes, no drawing. Go to EXIT_DRAW_SQUARE.


  ; 1) x coordiate validation (Test Fin.)
  LDR R0, R6, #0              ; Load “x coordinate” into R0.
  CMPI  R0, #0                ; “x coordinate” < 0 ?
  BRn APPLY_TOP_LEFT_CORNER   ; If yes, Go to APPLY_TOP_LEFT_CORNER.
  CMPIU R0, #127    	        ; “x coordinate” > 127 ?
  BRp APPLY_TOP_LEFT_CORNER   ; If yes, Go to APPLY_TOP_LEFT_CORNER.  

  ; 2) y coordiate validation (Test Fin.)
  LDR R0, R6, #1              ; Load “y coordinate” into R0.
  CMPI  R0, #0                ; “y coordinate” < 0 ?
  BRn APPLY_TOP_LEFT_CORNER   ; If yes, Go to APPLY_TOP_LEFT_CORNER.
  CMPIU R0, #123    	        ; “y coordinate” > 123 ?
  BRp APPLY_TOP_LEFT_CORNER   ; If yes, Go to APPLY_TOP_LEFT_CORNER.
  JMP AFTER_TOP_LEFT_CORNER   ; Passed every validations. Jump to AFTER_TOP_LEFT_CORNER.

  ; 3) Move (x,y) to (0,0) if needed!
  APPLY_TOP_LEFT_CORNER
  CONST R0, #0                ; Set R0 = 0
  STR R0, R6, #0              ; Store x coordiate = 0: dmem[xB000] = 0.
  STR R0, R6, #1              ; Store y coordiate = 0: dmem[xB001] = 0.
  AFTER_TOP_LEFT_CORNER

  ; 4) Set the display pointer (Test Fin.)
  LDR R0, R6, #0              ; Load "x coordinate" into R0.
  LDR R1, R6, #1              ; Load "y coordinate" into R1.
  LDR R2, R6, #4              ; Load the color of the rectangle into R2.

  ; Set the pixel pointer at R7.
  MUL R7, R1, R5              ; R7 = (y coordinate * NULL_COLS)
  ADD R7, R7, R0              ; R7 = (y coordinate * NULL_COLS) + x coordinate
  ADD R7, R7, R3              ; Add the starting address of video memory as an offset.  
  STR R7, R6, #6              ; Store curr pixel pointer at dmem[xB006]
  ;STR R2, R7, #0              ; Test) Color the pixel pointer R7 with the color R2.


  ;;; 6) Draw square by staking the vertical lines.
  ; Load length and width back again.
  LDR R3, R6, #2              ; Load the length of the rectangle into R3  
  LDR R4, R6, #3              ; Load the width of the regtangle into R4

  DRAW_SQUARE_LOOP
    CMPI R4, #0               ; R4 <= 0?
    BRnz EXIT_DRAW_SQUARE     ; If yes, go to EXIT_DRAW_SQUARE

    ; Get the area of the display at R0
    LC R0, OS_VIDEO_NUM_ROWS    ; Load NUM_ROWS at R0
    MUL R0, R0, R5              ; R0 = (NUM_ROWS * NUM_COLS)

    ; Get the bottom right pixel of the display at R1
    LEA R1, OS_VIDEO_MEM        ; R1 = OS_VIDEO_MEM
    ADD R1, R1, R0              ; R1 = OS_VIDEO_MEM + (NUM_ROWS * NUM_COLS) : the bottom right pixel
    CMP R7, R1                  ; (current pixel address) <= (the bottom right pixel) ?
    BRnz CAL_HORIZONTAL_END     ; If yes, no adjustment. Go to CAL_HORIZONTAL_END
      SUB R7, R7, R0            ; Else, adjust the pixel by subtracting (NUM_ROWS * NUM_COLS)   
      STR R7, R6, #6            ; Store curr pixel pointer at dmem[xB006] 

    ; Calculate the end point of each row in R0.
    CAL_HORIZONTAL_END
      ADD R0, R7, #0            ; Copy R7 at R0
      LEA R1, OS_VIDEO_MEM      ; R1 = OS_VIDEO_MEM
      SUB R0, R0, R1            ; R0 = (current pixel address) - OS_VIDEO_MEM
      DIV R0, R0, R5            ; R0 = ((current pixel address) - OS_VIDEO_MEM) / NUM_COLS
      ADD R0, R0, #1            ; R0 = (((current pixel address) - OS_VIDEO_MEM) / NUM_COLS) + 1
      MUL R0, R0, R5            ; R0 = ((((current pixel address) - OS_VIDEO_MEM) / NUM_COLS) + 1) * NUM_COLS
      ADD R0, R0, R1            ; R0 = (((((current pixel address) - OS_VIDEO_MEM) / NUM_COLS) + 1) * NUM_COLS) + OS_VIDEO_MEM

    ;;; Draw a horizontal line    
    DRAW_REC_VERT_LOOP
      CMPI R3, #0                   ; R3 <= 0 ?
      BRnz EXIT_DRAW_REC_VERT_LOOP  ; If yes, exit the loop

      ; Wrapping logic horizontal
      CMP R7, R0                    ; R7 < R0 ?
      BRn REGULAR_COLOR_PIXEL
        SUB R7, R7, R5              ; Adjust R7 by R7 = R7 - NUM_COLS
        STR R2, R7, #0              ; Color the pixel pointer R7 with the color R2.
        ADD R7, R7, R5              ; Adjust R7 by R7 = R7 + NUM_COLS
        JMP AFTER_COLOR_PIXEL

      REGULAR_COLOR_PIXEL
        STR R2, R7, #0              ; Color the pixel pointer R7 with the color R2.
        
      AFTER_COLOR_PIXEL
      ADD R7, R7, #1                ; R7++
      ADD R3, R3, #-1               ; R3--
      
      JMP DRAW_REC_VERT_LOOP
    EXIT_DRAW_REC_VERT_LOOP
    
    LDR R3, R6, #2            ; Load the length of the rectangle into R3
    ADD R4, R4, #-1           ; R4--
    SUB R7, R7, R3            ; Restore the x coordiate 
    ADD R7, R7, R5            ; R7 = R7 + NUM_COLS (Next row!)
    JMP DRAW_SQUARE_LOOP      ; Go back to the loop
  EXIT_DRAW_SQUARE

  LDR R7, R6, #5            ; Restore R7 = dmem[xB005]
  RTI       		            ; PC = R7 ; PSR[15]=0
