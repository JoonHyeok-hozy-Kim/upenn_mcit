/**************************************************************
 * file name   : program3.c                                   *
 * author      : Joon Hyeok Kim                               *
 * description : this C-file tests the functions of the       *
 *               main() with multiple integer/non-integer     *
 *               arguments                                    *
 *                                                            *
 *                                                            *
 **************************************************************
 *
 */

#include <stdio.h>
#include <string.h>

char* hozy_strcat(char* dest_end, char* str_ptr){
	/* Implemented to prevent redundant for loop at the beginning of my_strcat(). 
		Arguments - dest_end : Ending address of the target string to be concatenated to.
				  - str_ptr : Starting address of the string to concantenate with.
	*/
	while (*str_ptr){
		*dest_end = *str_ptr;	// Copy the value of str_ptr at dest_end
		dest_end++;				// Increment dest_end.
		str_ptr++;				// Increment str_ptr.
	}
	return dest_end;			// Return the final end address.
}

int main(int argc, char** argv) {

	// printf("# of arguments passed: %d\n", argc);
	int num_array[10];	// Array for the integer arguments
	int parsed_num;
	int num_idx = 0;

	char result_str[250] = "";		// String output for the non-integer arguments.
	char* result_end = result_str;	// Pointer at the end of result_str 
	char buffer_str[250];			// Temporary buffer for the non-integer arguments.
	char* buffer_ptr;				

	for (int i=0; i<argc; i++){
		/* Trim "./" from the first argument and store at result_str. */
		if (i == 0){
			sscanf(argv[i], "./%s", buffer_str);				// Trim "./" and store the string at buffer_str.
			buffer_ptr = buffer_str;							// Copy address of buffer_str at buffer_ptr.
			result_end = hozy_strcat(result_end, buffer_ptr);	// Call hozy_strcat() to concatenate and maintain result_str.
			continue;											// Go back to loop start.
		}

		/* Scan each argument and store it at the appropriate place. */
		if (sscanf(argv[i], "%d%s", &parsed_num, buffer_str) == 1){	// Scan with "%d%s" format and store integer at parsed_num and string at buffer_str.
			num_array[num_idx] = parsed_num;						// If only integer is identified, store that integer in num_array.
			num_idx++;												// Increment num_idx.
		} else {
			sprintf(buffer_str, " %s", argv[i]);				// Store the non-integer argument with the format of " %s" at buffer_str.
			buffer_ptr = buffer_str;							// Copy address of buffer_str at buffer_ptr.
			result_end = hozy_strcat(result_end, buffer_ptr);	// Call hozy_strcat() to concatenate and maintain result_str. 
		}
	}

	/* Print all integer arguments. */
	for (int i=0; i<num_idx; i++){
		printf("%d\n", num_array[i]);
	}

	/* Print all the non-integer arguments in the single string, result_str. */
	printf("%s\n", result_str);

	return (0);
}
