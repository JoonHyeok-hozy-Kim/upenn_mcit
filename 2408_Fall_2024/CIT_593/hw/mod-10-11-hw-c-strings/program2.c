/**************************************************************
 * file name   : program2.c                                   *
 * author      : Joon Hyeok Kim                               *
 * description : this C-file tests the functions of the       *
 *               "my_string" library                          *
 *                                                            *
 *                                                            *
 **************************************************************
 *
 */

#include <stdio.h>
#include <string.h>
#include "my_string.h"

int main() {

	char my_string[] = "Tom";

	/* Test for my_strrev() */
	printf("my_strrev() test : \"%s\"", my_string);		// Before
	printf(" -> \"%s\"\n", my_strrev(my_string));		// After
	printf("   reverse again : \"%s\"", my_string);		// Before
	printf(" -> \"%s\"\n\n", my_strrev(my_string));		// After
	

	/* Test for my_strccase() */
	printf("my_strccase() test : \"%s\"", my_string);	// Before
	printf(" -> \"%s\"\n", my_strccase(my_string));		// After
	printf("     convert again : \"%s\"", my_string);	// Before
	printf(" -> \"%s\"\n\n", my_strccase(my_string));	// After


	return (0);
}
