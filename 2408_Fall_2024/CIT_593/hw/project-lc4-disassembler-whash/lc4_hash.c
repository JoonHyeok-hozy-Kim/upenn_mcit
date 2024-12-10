/************************************************************************/
/* File Name : lc4_hash.c		 										*/
/* Purpose   : This file contains the definitions for the hash table  	*/
/*																		*/
/* Author(s) : tjf 														*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_hash.h"
#include "lc4_memory.h"

/*
 * creates a new hash table with num_of_buckets requested
 */
lc4_memory_segmented* create_hash_table (int num_of_buckets, 
             					         int (*hash_function)(void* table, void *key) ) 
{
	// Return NULL if num_of_buckets is invalid.
	if (num_of_buckets <= 0) return NULL;

	// allocate a single hash table struct
	lc4_memory_segmented* new_table = NULL;
	new_table = malloc(sizeof(lc4_memory_segmented));
	if (new_table == NULL) return NULL;

	new_table->num_of_buckets = num_of_buckets;

	// allocate memory for the buckets (head pointers)
	row_of_memory** new_buckets = NULL;
	new_buckets = malloc(sizeof(row_of_memory*) * num_of_buckets);
	if (new_buckets == NULL) {
		free(new_table);
		return NULL;
	}
	
	// Initialize buckets into NULL.
	for (int i=0; i<num_of_buckets; i++){
		new_buckets[i] = NULL;
	}

	// Assign the head of the newly generated Buckets to buckets field.
	new_table->buckets = new_buckets;

	// assign function pointer to call back hashing function
	new_table->hash_function = hash_function;

	return new_table;
}


/*
 * adds a new entry to the table
 */
int add_entry_to_tbl (lc4_memory_segmented* table, 
					  unsigned short int address,
			    	  unsigned short int contents) 
{
	// apply hashing function to determine proper bucket #
	int bucket_key = table->hash_function(table, &address);
	// Invalid hash value validation.
	if (bucket_key < 0 || bucket_key >= table->num_of_buckets) return -1;

	// add to bucket's linked list using linked list add_to_list() helper function
	int insert_result = 0;
	row_of_memory** target_bucket_head = table->buckets + bucket_key;
	insert_result = add_to_list(target_bucket_head, address, contents);
	// Invalid node addition validation
	if (insert_result != 0) return -2;
	
	return 0 ;
}

/*
 * search for an address in the hash table
 */
row_of_memory* search_tbl_by_address 	(lc4_memory_segmented* table,
			                   			 unsigned short int address ) 
{
	// apply hashing function to determine bucket # item must be located in
	row_of_memory* curr_row = NULL;
	int bucket_idx = table->hash_function(table, &address);

	// invoked linked_lists helper function, search_by_address() to return return proper node
	curr_row = search_address (table->buckets[bucket_idx], address);

	return curr_row;
}

/*
 * prints the linked list in a particular bucket
 */

void print_bucket (lc4_memory_segmented* table, 
				   int bucket_number,
				   FILE* output_file ) 
{
	// call the linked list helper function to print linked list
	if (table == NULL || bucket_number < 0 || bucket_number >= table->num_of_buckets){
		return;
	} else if (table->buckets[bucket_number] == NULL) return;

	print_lines(table->buckets[bucket_number], output_file);
	return;
}

/* Writes assembly on the given file */
int write_assy_on_file (lc4_memory_segmented* table, FILE* my_obj_file) {
	if (my_obj_file == NULL || table == NULL) return 1;
	row_of_memory* curr_node = NULL;
	unsigned short int prev_address = 0x0000;

	for (int bucket_idx=0; bucket_idx<table->num_of_buckets; bucket_idx++){
		curr_node = table->buckets[bucket_idx];
		if (curr_node == NULL) continue;

		// Print Directive: ".OS" if OS Code or OS Data but OS code not exists.
		if (bucket_idx == 2 || (bucket_idx == 3 && table->buckets[2] == NULL)){
			fprintf(my_obj_file, ".OS\n");
		}
		
		// Print Directive: ".CODE" if User Code or OS Code 
		if (bucket_idx % 2 == 0){			
			fprintf(my_obj_file, ".CODE\n");
		// Print Directive: ".DATA" if User Code or OS Code
		} else{
			fprintf(my_obj_file, ".DATA\n");
		}

		// Print Directive: ".ADDR" at the beginning of the bucket.
		fprintf(my_obj_file, ".ADDR x%04x\n\n", curr_node->address);

		// Print instructions.
		while (curr_node != NULL){
			// Print directives ".FALIGN" or ".ADDR UMM16" if node is not head and the address is not continuous.
			if (curr_node != table->buckets[bucket_idx] && curr_node->address - prev_address > 1){					
				// Print ".FALIGN" if the bucket is the Code, not head node, and tge address is a multiple of 16
				if (bucket_idx % 2 == 0 && curr_node->address % 0x10 == 0 && curr_node->address - prev_address <= 0x10){
					fprintf(my_obj_file, "\n.FALIGN\n");
				// Print ".ADDR" otherwise.
				} else{
					fprintf(my_obj_file, "\n.ADDR x%04x\n", curr_node->address);
				}
			}

			// Print label if exists.
			if (curr_node->label != NULL){
				fprintf(my_obj_file, "%s\n", curr_node->label);
			}

			if (curr_node->assembly != NULL){
				fprintf(my_obj_file, "\t%s\n", curr_node->assembly);
			}

			prev_address = curr_node->address;
			curr_node = curr_node->next;
		}

		fprintf(my_obj_file, "\n");	// Separator line
	}
	return 0;
}

/*
 * print the entire table (all buckets)
 */
void print_table (lc4_memory_segmented* table, 
				  FILE* output_file ) 
{
	// call the linked list helper function to print linked list to output file for each bucket
	if (table == NULL) return;

	// Print header once!
	print_header(output_file);

	// Print each buckets.
	for (int i=0; i<table->num_of_buckets; i++){
		print_bucket(table, i, output_file);
	}
	return;
}

/*
 * delete the entire table and underlying linked lists
 */

void delete_table (lc4_memory_segmented* table ) 
{
	// call linked list delete_list() on each bucket in hash table
	row_of_memory** curr_list_addr = NULL;
	for (int i=0; i<table->num_of_buckets; i++){
		curr_list_addr = &(table->buckets[i]);
		delete_list(curr_list_addr);
	}	
	free(table->buckets);

	// then delete the table itself
	free(table);
	return ;
}
