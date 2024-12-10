;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  file name   : user_draw.asm                            ;
;  author      : Joon Hyeok Kim
;  description : Draw the following 3 rectangles, coordinates are given in (x, y) order:
;                 - A red box, upper left coordinates: (50, 5), length = 10 and width 5
;                 - A green box, upper left coordinates: (10, 10), length = 50, width 40
;                 - A yellow box, upper left coordinates: (120, 100), length = 27, width 10
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The following CODE will go into USER's Program Memory
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


.CODE
.ADDR x0000
    
    ;; Test Case 1) 
    CONST R0, #50           ; “x coordinate” of upper-left corner of the rectangle.
    CONST R1, #5            ; “y coordinate” of upper-left corner of the rectangle.
    CONST R2, #10           ; length of the rectangle (in number of pixels).
    CONST R3, #5            ; width of the side of the rectangle (in number of pixels).
    CONST R4, x00           ; the color of the rectangle : Red
    HICONST R4, x7C         ; the color of the rectangle : Red
    TRAP x09                ; Call TRAP_DRAW_RECT.
    
    ;; Test Case 2) 
    CONST R0, #10           ; “x coordinate” of upper-left corner of the rectangle.
    CONST R1, #10           ; “y coordinate” of upper-left corner of the rectangle.
    CONST R2, #50           ; length of the rectangle (in number of pixels).
    CONST R3, #40           ; width of the side of the rectangle (in number of pixels).
    CONST R4, xE0           ; the color of the rectangle : Green
    HICONST R4, x07         ; the color of the rectangle : Green
    TRAP x09                ; Call TRAP_DRAW_RECT.
    
    ;; Test Case 3) 
    CONST R0, #120          ; “x coordinate” of upper-left corner of the rectangle.
    CONST R1, #100          ; “y coordinate” of upper-left corner of the rectangle.
    CONST R2, #27           ; length of the rectangle (in number of pixels).
    CONST R3, #10           ; width of the side of the rectangle (in number of pixels).
    CONST R4, xE0           ; the color of the rectangle : Yello
    HICONST R4, xFF         ; the color of the rectangle : Yello
    TRAP x09                ; Call TRAP_DRAW_RECT.
    

    ; Edge Case 0)
    ;CONST R0, #60           ; x coordinate = 60
    ;CONST R1, #120          ; y corrdinate = 120
    ;CONST R2, #10           ; length = #10
    ;CONST R3, #10           ; width = #10
    ;CONST R4, xFF           ; the color of the rectangle : Turquoise 
    ;HICONST R4, x07         ; the color of the rectangle : Turquoise 
    ;TRAP x09                ; Call TRAP_DRAW_RECT.


    ;;; Edge Case 1)
    ;CONST R0, #-10          ; “x coordinate” = #0
    ;CONST R1, #-20          ; “y coordinate” = #0
    ;CONST R2, #40           ; length = #5
    ;CONST R3, #60           ; width  = #5
    ;CONST R4, x19           ; the color of the rectangle : Pink
    ;HICONST R4, xFE         ; the color of the rectangle : Pink
    ;TRAP x09                ; Call TRAP_DRAW_RECT.

    ;;; Edge Case 2)
    ;CONST R0, #120          ; “x coordinate” = #120
    ;CONST R1, #110          ; “y coordinate” = #110
    ;CONST R2, #40           ; length = #40
    ;CONST R3, #30           ; width  = #30
    ;CONST R4, x04           ; the color of the rectangle : Blood Orange
    ;HICONST R4, xD8         ; the color of the rectangle : Blood Orange
    ;TRAP x09                ; Call TRAP_DRAW_RECT.

    ;;; Edge Case 3)
    ;CONST R0, #100          ; “x coordinate” = #100
    ;CONST R1, #100          ; “y coordinate” = #100
    ;CONST R2, #128          ; length = #128
    ;CONST R3, #124          ; width = #124
    ;CONST R4, xFF           ; the color of the rectangle : Turquoise 
    ;HICONST R4, x07         ; the color of the rectangle : Turquoise 
    ;TRAP x09                ; Call TRAP_DRAW_RECT.

    ;;; Edge Case 4)
    ;CONST R0, #100          ; “x coordinate” = #100
    ;CONST R1, #100          ; “y coordinate” = #100
    ;CONST R2, #129          ; length = #129
    ;CONST R3, #124          ; width = #124
    ;CONST R4, x00           ; the color of the rectangle : Black 
    ;HICONST R4, x00         ; the color of the rectangle : Black 
    ;TRAP x09                ; Call TRAP_DRAW_RECT.

    ;;; Edge Case 5)
    ;CONST R0, #100          ; “x coordinate” = #100
    ;CONST R1, #100          ; “y coordinate” = #100
    ;CONST R2, #128          ; length = #128
    ;CONST R3, #125          ; width = #125
    ;CONST R4, xFF           ; the color of the rectangle : White 
    ;HICONST R4, xFF         ; the color of the rectangle : White 
    ;TRAP x09                ; Call TRAP_DRAW_RECT.

END