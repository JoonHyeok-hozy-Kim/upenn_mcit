    /************************************************************************/
/* File Name : lc4_loader.c		 										*/
/* Purpose   : This file implements the loader (ld) from PennSim		*/
/*             It will be called by main()								*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_memory.h"
#include "lc4_hash.h"

/* declarations of functions that must defined in lc4_loader.c */

FILE* open_file(char* file_name)
{
	FILE* target_file = NULL;
	target_file = fopen(file_name, "rb");
	return target_file;
}


/* Opens file with writing mode. */ 
FILE* open_write_file(char* file_name) {
	FILE* target_file = NULL;
	target_file = fopen(file_name, "w");
	return target_file;
}

/* Modify (Big <-> Little) Endian */
unsigned short int endian_treatment(unsigned short int original){
	unsigned short int result = ((original >> 8) | (original << 8)) & 0xffff;
	return result;
}

int parse_file (FILE* my_obj_file, lc4_memory_segmented* memory)
{
	// Declare variables that will be assigned to the memory structure.
	unsigned short int header_type;
	unsigned short int address;
	unsigned short int n;
	unsigned short int content;

	// Variables for symbolic labels.
	char symbol_str[100];
	int symbol_idx;
	int add_entry_check;
	int return_val = 0;

    while (1){
		/* Parse header_type */
		if (fread(&header_type, sizeof(unsigned short int), 1, my_obj_file) != 1) break;
		header_type = endian_treatment(header_type);	// Endian treatment.

		/* Parse address */
		if (fread(&address, sizeof(unsigned short int), 1, my_obj_file) != 1) break;
		address = endian_treatment(address);	// Endian treatment.

		/* Parse n */
		if (fread(&n, sizeof(unsigned short int), 1, my_obj_file) != 1) break;
		n = endian_treatment(n);	// Endian treatment.

		if (header_type == 0xCADE || header_type == 0xDADA){	// Case 1) Code and Data
			for (int i=0; i<n; i++){
				if (fread(&content, sizeof(unsigned short int), 1, my_obj_file) != 1) break;
				content = endian_treatment(content);	// Endian treatment.

				// Add node using address and content.
				if(add_entry_to_tbl(memory, address, content) != 0){
					printf("[Parse File Add Code/Data Entry Error] address : %x, content : %x\n", address, content);
					return_val = -1;
					break;
				}

				address++;	// Increment the address.
			}
		} else if (header_type == 0xC3B7){	// Case 2) Symbol
			symbol_idx = 0;
			for (int i=0; i<n; i++){
				// Validate the symbolic binary.
				if (fread(symbol_str+i, sizeof(char), 1, my_obj_file) != 1) break;
			}
			symbol_str[n] = '\0';
			row_of_memory* target_row = search_tbl_by_address(memory, address);	// Find node by address.
			
			/* Add new node if the label' address has not been assigned yet. */
			if (target_row == NULL){
				if(add_entry_to_tbl(memory, address, 0) != 0){	// Initialize content to zero.
					printf("[Parse File Add Symbol Entry Error] address : %x, label : %s\n", address, symbol_str);
					return_val = -4;
					break;
				}
				target_row = search_tbl_by_address(memory, address);
			}

			// Assign label to the targetted node.
			target_row->label = malloc(sizeof(char) * strlen(symbol_str)+1);
			if (target_row->label == NULL){				
				printf("[Parse File Error] Symbol malloc failure.\n");
				return_val = -5;
				break;
			}

			strcpy(target_row->label, symbol_str);
			// printf("[Symbol] address : %x, label : %s\n", address, symbol_str);
		} else{
			printf("[Parse File Error] Unexpected Header Type : %04x\n", header_type);
			fclose(my_obj_file);
			return_val = -3;
			break;
		}
	}
	
	return return_val ;
}
