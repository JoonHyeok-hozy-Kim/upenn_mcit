/********************************************************
 * file name   : problem5.c                             *
 * author      : Joon Hyeok Kim
 * description : C program to call LC4-Assembly TRAP_DRAW_REC
 *               the TRAP is called through the wrapper 
 *               lc4putc() (located in lc4_stdio.asm)   *
 ********************************************************
*
*/

/*
;;; TRAP x09 Inputs - R0 = “x coordinate” of upper-left corner of the rectangle.
;;;                 - R1 = “y coordinate” of upper-left corner of the rectangle.
;;;                 - R2 = length of the rectangle (in number of pixels).
;;;                 - R3 = width of the side of the rectangle (in number of pixels).
;;;                 - R4 = the color of the rectangle
;;;          Output - none
*/

int main() {

	int rectangle1[6] = {50, 5, 10, 5, 0x00, 0x7C};     // top_left_coordinate = ( 50,   5), length = 10, width =  5, color red
    int rectangle2[6] = {10, 10, 50, 40, 0xE0, 0x07};   // top_left_coordinate = ( 10,  10), length = 50, width = 40, color green
    int rectangle3[6] = {120, 100, 27, 10, 0xE0, 0xFF}; // top_left_coordinate = (120, 100), length = 27, width = 10, color yellow

    lc4_draw_rec(rectangle1);   // Call lc4_draw_rec with rectangle1
    lc4_draw_rec(rectangle2);   // Call lc4_draw_rec with rectangle2
    lc4_draw_rec(rectangle3);   // Call lc4_draw_rec with rectangle3
	
	return (0) ;

}
