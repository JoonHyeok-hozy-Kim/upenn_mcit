/**************************************************************
 * file name   : program4.c                                   *
 * author      : Joon Hyeok Kim                               *
 * description : this C-file tests my_strtok				  *
 *                                                            *
 *                                                            *
 **************************************************************
 *
 */

#include <stdio.h>
#include <string.h>
#include "my_string.h"

void strtok_tester(char* str, const char* delim){
	printf("Test my_strtok(\"%s\", \"%s\")\n", str, delim);
	char* token = my_strtok(str, delim);			// Initial call.
	int i = 0;
	while (token != NULL){							// Repeat until the terminating condition.
		printf(" Token %d : %s\n", ++i, token);		// Print the value in token
		token = my_strtok(NULL, delim);				// Next call.
	}
	printf("Fin Test.\n\n");
	return;
}

int main() {
	/* Test Case 1 */
	char str1[40] = "abc - def-hij--k-";
	char delim1[5] = "-";
	strtok_tester(str1, delim1);

	/* Test Case 2 */
	char str2[40] = "abc.def,ghi_jkl^mon*pqr";
	char delim2[5] = "._*";
	strtok_tester(str2, delim2);

	/* Test Case 3 */
	char str3[40] = "/.//.///./../..././././";
	char delim3[5] = "./.";
	strtok_tester(str3, delim3);

	/* Test Case 4 */
	char str4[10] = "aaaa";
	char delim4[5] = "b";
	strtok_tester(str4, delim4);

	/* Test Case 5 */
	char str5[10] = "aa";
	char delim5[5] = "aaa";
	strtok_tester(str5, delim5);

	/* Test Case 6 */
	char str6[10] = "aaaaab";
	char delim6[5] = "a";
	strtok_tester(str6, delim6);


	return (0);
}
