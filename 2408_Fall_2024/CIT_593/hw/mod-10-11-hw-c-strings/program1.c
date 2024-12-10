/**************************************************************
 * file name   : program1.c                                   *
 * author      : tjf & you                                    *
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

	// a string we will test with
	char my_string [100] = "Tom" ;

	// test of strlen() functions 
	printf ("Sample string: \"%s\" \n", my_string) ;
	printf (" string's length: strlen()     = %lu \n",   strlen  (my_string)) ;
	printf (" string's length: my_strlen()  = %lu \n", my_strlen (my_string)) ;
	printf (" string's length: my_strlen2() = %lu \n", my_strlen2(my_string)) ;

    /* CIT 593 students: TODO: add code to test your my_string.h functions */

	/* Test for my_strcpy() & my_strcpy2() */
	char copy_to1[100];
	char copy_to2[100];
	printf ("\n<my_strcpy1 Test> Sample string: \"%s\" \n", my_string) ;
	printf (" string copy: my_strcpy1() = %s \n", my_strcpy(copy_to1, my_string));
	printf ("   double check with label = %s \n", copy_to1);
	printf (" string copy: my_strcpy2() = %s \n", my_strcpy2(copy_to2, my_string));
	printf ("   double check with label = %s \n", copy_to2);

	/* Test for my_strchr() & my_strchr2() */
	printf ("\n<my_strchr Test> Sample string: \"%s\" \n", my_string) ;
	char in_tom = 'o';			// 'o' is included in "Tom".
	char not_in_tom = 'z';		// 'z' is not included in "Tom".
	printf("[my_strchr] string after '%c' is \"%s\"\n", in_tom, my_strchr(my_string, in_tom));
	printf("[my_strchr] string after '%c' is \"%s\"\n", not_in_tom, my_strchr(my_string, not_in_tom));
	printf("[my_strchr2] string after '%c' is \"%s\"\n", in_tom, my_strchr2(my_string, in_tom));
	printf("[my_strchr2] string after '%c' is \"%s\"\n", not_in_tom, my_strchr2(my_string, not_in_tom));

	/* Test for my_strcat() & my_strchr2() */
	char concat_str1[100] = "Professor ";
	char concat_str2[100] = "Professor ";
	printf ("\n<my_strcat Test> Sample string: \"%s\" will be concatenated with \"%s\" \n", concat_str1, my_string) ;
	printf(" concatenated by my_strcat() : \"%s\"\n", my_strcat(concat_str1, my_string));
	printf(" concatenated by my_strcat2() : \"%s\"\n", my_strcat2(concat_str2, my_string));

	/* Test for strcmp() & strcmp2() */
	char* cmp_strs1[] = {"Tom", "Toy", "Mom"};
	printf ("\n<strcmp Test>");
	printf("\n [strcmp() Test with cmp_strs1]\n");
	for (int i=0; i<3; i++){
		printf("%s", my_string);
		int cmp = strcmp(my_string, cmp_strs1[i]);
		if (cmp == 0) {
			printf(" = ");
		} else if (cmp < 0) {
			printf(" < ");
		} else {
			printf(" > ");
		}
		printf("%s\n", cmp_strs1[i]);
	}

	printf("\n [strcmp2() Test with cmp_strs1]\n");
	for (int i=0; i<3; i++){
		printf("%s", my_string);
		int cmp = strcmp2(my_string, cmp_strs1[i]);
		if (cmp == 0) {
			printf(" = ");
		} else if (cmp < 0) {
			printf(" < ");
		} else {
			printf(" > ");
		}
		printf("%s\n", cmp_strs1[i]);
	}
	
	char* cmp_strs2[] = {"To", "Tom", "Tommy"};
	printf("\n [strcmp() Test with cmp_strs2]\n");
	for (int i=0; i<3; i++){
		printf("%s", my_string);
		int cmp = strcmp(my_string, cmp_strs2[i]);
		if (cmp == 0) {
			printf(" = ");
		} else if (cmp < 0) {
			printf(" < ");
		} else {
			printf(" > ");
		}
		printf("%s\n", cmp_strs2[i]);
	}
	
	printf("\n [strcmp2() Test with cmp_strs2]\n");
	for (int i=0; i<3; i++){
		printf("%s", my_string);
		int cmp = strcmp2(my_string, cmp_strs2[i]);
		if (cmp == 0) {
			printf(" = ");
		} else if (cmp < 0) {
			printf(" < ");
		} else {
			printf(" > ");
		}
		printf("%s\n", cmp_strs2[i]);
	}
	
	char cmp_test3_1[] = "Ab";
	char cmp_test3_2[] = "ac";
	printf("\n [strcmp() Test with cmp_test3]\n");
	printf("%s", cmp_test3_1);
	int cmp = strcmp(cmp_test3_1, cmp_test3_2);
	if (cmp == 0) {
		printf(" = ");
	} else if (cmp < 0) {
		printf(" < ");
	} else {
		printf(" > ");
	}
	printf("%s\n", cmp_test3_2);

	printf("\n [strcmp2() Test with cmp_test3]\n");
	printf("%s", cmp_test3_1);
	cmp = strcmp2(cmp_test3_1, cmp_test3_2);
	if (cmp == 0) {
		printf(" = ");
	} else if (cmp < 0) {
		printf(" < ");
	} else {
		printf(" > ");
	}
	printf("%s\n", cmp_test3_2);
	
	char cmp_test3_3[] = "ab";
	char cmp_test3_4[] = "Ac";
	printf("\n [strcmp() Test with cmp_test3]\n");
	printf("%s", cmp_test3_3);
	cmp = strcmp(cmp_test3_3, cmp_test3_4);
	if (cmp == 0) {
		printf(" = ");
	} else if (cmp < 0) {
		printf(" < ");
	} else {
		printf(" > ");
	}
	printf("%s\n", cmp_test3_4);

	printf("\n [strcmp2() Test with cmp_test3]\n");
	printf("%s", cmp_test3_3);
	cmp = strcmp2(cmp_test3_3, cmp_test3_4);
	if (cmp == 0) {
		printf(" = ");
	} else if (cmp < 0) {
		printf(" < ");
	} else {
		printf(" > ");
	}
	printf("%s\n", cmp_test3_4);

	return (0) ;

}
