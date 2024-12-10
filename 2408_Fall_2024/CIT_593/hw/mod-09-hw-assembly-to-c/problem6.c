/********************************************************
 * file name   : problem6.c                             *
 * author      : Joon Hyeok Kim
 * description : C program to play PONG
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

	int rectangle[6] = {60, 100, 20, 5, 0xE0, 0xFF}; // top_left_coordinate = (60, 100), length = 20, width = 5, color yellow
	int erase[6]     = {60, 100, 20, 5, 0x00, 0x00}; // top_left_coordinate = (60, 100), length = 20, width = 5, color black
    char control = 0;

    lc4_draw_rec(rectangle);            // Display the initial rectangle

    while (1){    
        control = lc4_getc();           // Get user input
        if (control == 0x1B){
            break;                      // Exit game if user input is esc.
        } else{
            if (control == 'j' || control == 'J'){  // Consider both the upper and lower case
                rectangle[0] -= 10;     // Move left by 10
                //  lc4_reset_vmem();   // Not working...
                lc4_draw_rec(erase);    // Draw a black rectangle to erase the previous one
                erase[0] -= 10;         // Prepare for the next erase
            } else if (control == 'k' || control == 'K'){   // Consider both the upper and lower case
                rectangle[0] += 10;     // Move right by 10
                //  lc4_reset_vmem();   // Not working...
                lc4_draw_rec(erase);    // Draw a black rectangle to erase the previous one
                erase[0] += 10;         // Prepare for the next erase
            } else{
                continue;               // Otherwise, loop back and get the next user input
            }
            lc4_draw_rec(rectangle);    // Draw the new rectangle
        }
    }
	
	return (0) ;

}
