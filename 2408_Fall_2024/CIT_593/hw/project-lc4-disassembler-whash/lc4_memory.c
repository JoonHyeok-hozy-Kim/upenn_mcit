/************************************************************************/
/* File Name : lc4_memory.c		 										*/
/* Purpose   : This file implements the linked_list helper functions	*/
/* 			   to manage the LC4 memory									*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lc4_memory.h"


/*
 * adds a new node to linked list pointed to by head
 */
int add_to_list (row_of_memory** head,
		 		 unsigned short int address,
		 		 unsigned short int contents)
{
	// printf("address : %04x, content : %04x\n", address, contents);
	/* allocate memory for a single node */
	row_of_memory* prev = NULL;
	row_of_memory* new_memory = NULL;



	new_memory = malloc(sizeof(row_of_memory));
	// Return -1 if malloc fails.
	if (new_memory == NULL){
		return -1;
	}

	/* populate fields in newly allocated node with arguments: address/contents */
	new_memory->address = address;
	new_memory->contents = contents;

    /* make certain to set other fields in structure to NULL */
	new_memory->label = NULL;
	new_memory->assembly = NULL;
	new_memory->next = NULL;

	/* if head==NULL, node created is the new head of the list! */
	if (*head == NULL){
		*head = new_memory;
	}
	/* If the address overlaps, overwrite the data! (Ed #748) */
	else if (address == (*head)->address){
			// Overwrite new data
			(*head)->address = address;
			(*head)->contents = contents;

			// Free new_memory
			free(new_memory);
	} 
	/* Case when the inserted node's value is less than the current head. */
	else if (address < (*head)->address){
		new_memory->next = (*head);		// Make head be the next of new_memory
		*head = new_memory;				// Make new_memory as the new head of this linked list.
	} 
	/* otherwise, insert the node into the linked list in order of ascending addresses */
	else{
		prev = *head;
		while (prev->next != NULL){
			/* If the address overlaps, overwrite the data! (Ed #748) */
			if (address == prev->next->address) {					
				// Overwrite new data
				prev->next->address = address;
				prev->next->contents = contents;

				// Free new_memory
				free(new_memory);

				return 0;

			} else if (address < prev->next->address){	// Traverse until we find the proper spot!
				break;
			}
			prev = prev->next;
		}
		new_memory->next = prev->next;		// Link prev's next into new_memory's next
		prev->next = new_memory;			// Link new_memory to be the next of prev
	}

	/* return 0 for success, -1 if malloc fails */
	return 0 ;
}


/*
 * search linked list by address field, returns node if found
 */
row_of_memory* search_address (row_of_memory* head,
			        		   unsigned short int address )
{
	/* traverse linked list, searching each node for "address"  */
	while (head != NULL){
		/* return pointer to node in the list if item is found */
		if (head->address == address){
			return head;
		}
		head = head->next;
	}	

	/* return NULL if list is empty or if "address" isn't found */
	return NULL ;
}

/*
 * search linked list by opcode field, returns node if found
 */
row_of_memory* search_opcode (row_of_memory* head,
				      		  unsigned short int opcode  )
{
	/* traverse linked list until node is found with matching opcode
	   AND "assembly" field of node is empty */
	while (head != NULL){
		// printf("contents : %04x, trimmed : %0x\n", head->contents, head->contents >> 12);
		if (head->contents >> 12 == opcode && head->assembly == NULL){
			break;
		}
		head = head->next;
	   }

	/* return pointer to node in the list if item is found */
	/* return NULL if list is empty or if no matching nodes */
	return head;
}

/*
 * delete the node from the linked list with matching address
 */
int delete_from_list (row_of_memory** head,
			          unsigned short int address ) 
{
	/* if head isn't NULL, traverse linked list until node is found with matching address */
	row_of_memory* prev = NULL;
	row_of_memory* curr = *head;

	while (1){
		if (curr == NULL){
			return -1;			// Search failure : return -1.
		} else if (curr->address == address){
			break;				// Found the target! Break the loop.
		}
		prev = curr;			// Put curr to be prev
		curr = curr->next;		// Proceed with the node.
	}

	/* delete the matching node, re-link the list */
	if (prev == NULL){
		*head = curr->next;			// Case when delete target is head.
	} else {
		prev->next = curr->next;	// Linke prev with curr's next.
	}

	/*
	Fields that should be freed!
		char * label ;
		char * assembly ;
	*/
	if (curr->label != NULL) free(curr->label);			// Free label if needed.
	if (curr->assembly != NULL)free(curr->assembly);	// Free assembly if needed.
	free(curr);						// Free the entire node.

	/* make certain to update the head pointer - if original was deleted */

	/* return 0 if successfully deleted the node from list, -1 if node wasn't found */
	return 0 ;

}

/* Created for code reuse. */
void print_header(FILE* output_file){
	fprintf(output_file, "%-25s", "<label>");		// Header Format 25 fixed for label
	fprintf(output_file, "%-20s", "<address>");	// Header Format 20 fixed for address
	fprintf(output_file, "%-20s", "<contents>");	// Header Format 20 fixed for contents
	fprintf(output_file, "%-20s\n", "<assembly>");	// Header Format 20 fixed for assembly
}

/* Created for code reuse. */
void print_lines(row_of_memory* head, FILE* output_file){
	while (head != NULL){
		if (head->label != NULL){
			fprintf(output_file, "%-25s", head->label);		// Line Format 25 fixed for label
		} else{
			fprintf(output_file, "%-25s", "");					// Line Format 25 fixed for label
		}
		fprintf(output_file, "%04x%-16s", head->address, "");	// Line Format 20 fixed for address
		fprintf(output_file, "%04x%-16s", head->contents, "");	// Line Format 20 fixed for contents
		if (head->assembly != NULL){
			fprintf(output_file, "%-20s\n", head->assembly);	// Line Format 20 fixed for assembly
		} else{
			fprintf(output_file, "%-20s\n", "");				// Line Format 20 fixed for assembly
		}
		head = head->next;
	}
	return;
}

void print_list (row_of_memory* head, FILE* output_file )
{
	/* make sure head isn't NULL */
	if (head == NULL || output_file == NULL){
		return;
	}
	
	/* print out a header to output_file */
	print_header(output_file);

	/* traverse linked list, print contents of each node to output_file */
	print_lines(head, output_file);

	return ;
}

/*
 * delete entire linked list
 */
void delete_list (row_of_memory** head )
{
	/* delete entire list node by node */
	/* set head = NULL upon deletion */
	row_of_memory* curr = *head;
	row_of_memory* temp = NULL;
	while (curr != NULL){
		temp = curr->next;				// Save next node before freeing the entire node.

		if (curr->label != NULL) free(curr->label);			// Free label if needed.
		if (curr->assembly != NULL) free(curr->assembly);	// Free assembly if needed.
		free(curr);						// Free the entire node
		curr = temp;					// Proced with the next node.
	}
	*head = NULL;						// Following the rule, nullify the head as the empty linked list.
	return ;
}