/************************************************************************/
/* File Name : lc4_memory_test.c		 								*/
/* Purpose   : This file is for part1 of the assignment only			*/
/* 			   It allows you to test functions in lc4_memory.c			*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"


int main ()
{

    /* This main() is simply to test the basic functionalty of lc4_memory.c for part 1.
       It must not be used for part 2 of the assignment
    */
    int function_check = 0;

    /* Step 1: create a local linked list head pointer and allocate memory to it using 'add_to_list' */
	row_of_memory* test_list = NULL ;
    row_of_memory** head_pointer = &test_list;
    // check_add_to_list = add_to_list(test_list)

    /* Step 2: test your linked list by adding some 'dummy' data: 
               add_to_list(..., 0x0000, 0x1234), add_to_list(..., 0x0001, 0x5678), ... */
    function_check = add_to_list(head_pointer, 0x0000, 0x0001);
    function_check = add_to_list(head_pointer, 0x0001, 0x0002);
    
	/* Step 3: print your list, see it works! */
    print_list(*head_pointer, stdout);

    /* Step 4: try adding data to the list out of order: 
             add_to_list(..., 0x0003, 0x9ABC), add_to_list(..., 0x0002, 0xDEF0) */
    function_check = add_to_list(head_pointer, 0x0006, 0x0003);
    function_check = add_to_list(head_pointer, 0x0004, 0x0004);
    function_check = add_to_list(head_pointer, 0x0005, 0x0005);
    function_check = add_to_list(head_pointer, 0x0003, 0x0006);
    function_check = add_to_list(head_pointer, 0x0002, 0x0007);

    /* Step 5: print your list, ensure it is in ascedning order by address */
    print_list(*head_pointer, stdout);
    
	/* Step 6: delete a single node, say the one with address: 0x0002 */
    function_check = delete_from_list(head_pointer, 0x0002);
    function_check = delete_from_list(head_pointer, 0x0004);
    // if (function_check != 0) {printf("delete not working!\n");}

    /* Step 7: print your list (to FILE:stdout), ensure that the list is still intact */
    print_list(*head_pointer, stdout);

	/* Step 7-1: Overwrite address 0x0000 and address 0x0005 */
    function_check = add_to_list(head_pointer, 0x0000, 0xffff);
    function_check = add_to_list(head_pointer, 0x0005, 0xeeee);
    print_list(*head_pointer, stdout);


    /* Step 8: try deleting the first node in the list and then 
               print your list, ensure that the list is still intact */
    function_check = delete_from_list(head_pointer, 0x0000);
    // if (function_check != 0) {printf("delete not working!\n");}
    print_list(*head_pointer, stdout);

    /* Step 9: delete any remaining nodes
               try printing your list, it should not print anything out */
    delete_list(head_pointer);
    printf("After delete_list!\n");
    print_list(*head_pointer, stdout);

    /* Step 10: when finished, run valgrind on your program to see if there are any leaks
               points will be lost even if your program is working but leaking memory */

	return 0;
}