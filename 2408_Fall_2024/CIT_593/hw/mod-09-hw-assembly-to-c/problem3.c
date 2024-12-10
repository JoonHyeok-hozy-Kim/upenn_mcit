/********************************************************
 * file name   : problem3.c                             *
 * author      : Joon Hyeok Kim
 * description : C program to call LC4-Assembly TRAP_GETC and TRAP_PUTC
 *               the TRAP is called through the wrapper 
 *               lc4putc() (located in lc4_stdio.asm)   *
 ********************************************************
*
*/

int main() {

	char c = 0;	    // Declare a char variable c and initialize it into 0.

    while (c != 0x0A){  // While c != line feed (enter)
        c = lc4_getc(); // Get input from the keyboard.
        lc4_putc(c);    // Print character on the display.
    }
	
	return (0) ;

}
