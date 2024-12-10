/************************************************************************/
/* File Name : lc4_loader.h		 										*/
/* Purpose   : This file declares the functions for lc4_loader.c		*/
/*             															*/
/*             															*/
/* Author(s) : tjf														*/
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"
#include "lc4_hash.h"

/* declarations of functions that must defined in lc4_loader.c */



/**
 * opens up name of the file passed in, returns a pointer
 * to the open file
 *
 * returns the FILE pointer upon success, else NULL.
 */
FILE* open_file(char* file_name) ;

/**
 * opens up name of the file passed in, returns a pointer
 * to the open a file to write on. 
 * Implemented independently, since we cannot modify the open_file function.
 *
 * returns the FILE pointer upon success, else NULL.
 */
FILE* open_write_file(char* file_name) ;

/**
 * Endianness treatment. Convert big(small)-endian into small(big)-endian
*/
unsigned short int endian_treatment(unsigned short int original);


/**
 * parses the given input file into hashtable
 *
 * returns 0 upon successs, non-zero if an error occurs.
 */
int parse_file (FILE* my_obj_file, lc4_memory_segmented* memory) ;
