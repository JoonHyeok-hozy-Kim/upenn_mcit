/********************************************************
 * file name   : problem4.c                             *
 * author      : Joon Hyeok Kim
 * description : C program to call LC4-Assembly TRAP_PUTS
 *               the TRAP is called through the wrapper 
 *               lc4putc() (located in lc4_stdio.asm)   *
 ********************************************************
*
*/

int main() {

	char love[] = {'I', ' ', 'L', 'O', 'V', 'E', ' ', 'C', 'I', 'T', '5', '9', '3', 0x00};
	// char* love = "I LOVE CIT593";

    lc4_puts(love); // Call lc4_puts
	
	return (0) ;

}
