/************************************************************************/
/* File Name : lc4.c 													*/
/* Purpose   : This file contains the main() for this project			*/
/*             main() will call the loader and disassembler functions	*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "lc4_memory.h"
#include "lc4_hash.h"
#include "lc4_loader.h"
#include "lc4_disassembler.h"
#define TXT_EXTENSION ".txt"
#define OBJ_EXTENSION ".obj"

/* program to mimic pennsim loader and disassemble object files */
// Custom Hash Function
int hozy_hash_function(void* table, void* key){
	unsigned short int key_val = *((unsigned short int*) key);

	int temp = key_val / 0x1000;
	if (temp <= 1){			// 0x0000 ~ 0x1FFF : User Code
		return 0;
	} else if (temp <= 7){	// 0x2000 ~ 0x7FFF : User Data
		return 1;
	} else if (temp <= 9){	// 0x8000 ~ 0x9FFF : OS Code
		return 2;
	} else{					// 0xA000 ~ 0xFFFF : OS Data
		return 3;
	}
}

// Custom Hash Function Tester Function
void test_hozy_hash_function(){
	printf("[test_hozy_hash_function]\n");
	lc4_memory_segmented* fake_table = NULL;
	fake_table = malloc(sizeof(lc4_memory_segmented));
	if (fake_table == NULL) return;

	fake_table->num_of_buckets = 4;
	unsigned short int  test_addresses[12] = {
		0x0000, 0x1000, 0x1FFF, 				// bucket 0
		0x2000, 0x345A, 0x7FFF, 				// bucket 1
		0x8000, 0x8A9D, 0x9FFF, 				// bucket 2
		0xA000, 0xBCDE, 0xFFFF  				// bucket 3
	};

	unsigned short int* key_ptr = test_addresses;
	for (int i=0; i<12; i++){
		printf("Address : 0x%04x, Bucket : %d\n", test_addresses[i], hozy_hash_function(fake_table, key_ptr++));
	}

	free(fake_table);
	return;
}

int ends_with(const char* str, const char* src){
	regex_t regex;
	int ret;
	char* pattern = NULL;
	pattern = malloc(sizeof(char) * (2+ strlen(src) + 2));	// Regex Pattern : \\<src>$
	if (pattern == NULL){
		printf("[Regex Processing Error] Malloc fail while generating pattern.\n");
		return -1;
	}
	sprintf(pattern, "\\");
	strcat(pattern, src);
	strcat(pattern, "$");

	ret = regcomp(&regex, pattern, REG_EXTENDED);
	if (ret) {
		printf("[Regex Processing Error] Could not compile regex\n");
		return 0;
	}

	ret = regexec(&regex, str, 0, NULL, 0);
	regfree(&regex);
	free(pattern);

    if (!ret) {
        return 0; // Match Success!
    } else {
        return 1; // Not matched.
    }
}

/* [Ed #771] Argument Format Validation Check */
int argument_validation(int argc, char** argv){
	if (argc < 2){
		printf("[Argument Validation Error] No argument passed. Please input at least two arguments.\n");
		return 1;
	} else if (argc < 3){
		printf("[Argument Validation Error] Only one argument passed. Please input at least two arguments. \n");
		return 1;
	}

	// Check whether the first argument ends with ".txt"
	if (ends_with(argv[1], TXT_EXTENSION) != 0){
		printf("[Argument Validation Error] First argument must be in \"<file_name>%s\" format but \"%s\" was given.\n", TXT_EXTENSION, argv[1]);
		return 1;
	}
	// Check whether the arguments from the second end with ".obj"
	for (int i=2; i<argc; i++){
		if (ends_with(argv[i], OBJ_EXTENSION) != 0){
			printf("[Argument Validation Error] %d-th argument must be in \"<file_name>%s\" format but \"%s\" was given.\n", i, OBJ_EXTENSION, argv[i]);
			return 1;
		}
	}

	return 0;
}

int main (int argc, char** argv) {

	/**
	 * main() holds the hashtable &
	 * only calls functions in other files
	 */

	/* Test the custom hash function. */
	// test_hozy_hash_function();
	int pgm_run;

	/* step 1: create a pointer to the hashtable: memory 	*/
	lc4_memory_segmented* memory = NULL ;

	/* step 2: call create_hash_table() and create 4 buckets 	*/
	memory = create_hash_table(4, &hozy_hash_function);

	/* step 3: determine filename, then open it		*/
	/*   TODO: extract filename from argv, pass it to open_file() */
	if (argument_validation(argc, argv) != 0) return -1;		// [Ed #771] : Argument Format Validation

	// Open outputfile that we will write hash table on.
	char write_file_name[100];
	FILE* write_file = NULL;
	sprintf(write_file_name, "%s", argv[1]);
	write_file = open_write_file(write_file_name);
	if (write_file == NULL){
		printf("[write_file Open Error] File name : %s\n", write_file_name);
		return -1;
	}

	// Create an asm file to write the assembly.
	char asm_file_name[100];
	sprintf(asm_file_name, "");
	char* asm_file_temp = strtok(write_file_name, ".");
	strcat(asm_file_name, asm_file_temp);
	strcat(asm_file_name, ".asm");
	// printf("asm_file_name : %s\n", asm_file_name);
	FILE* asm_write_file = NULL;
	asm_write_file = open_write_file(asm_file_name);
	if (write_file == NULL){
		printf("[asm_write_file Open Error] File name : %s\n", asm_file_name);
		return -1;
	}

	/* step 4 and step 5 incorporated. */
	/* step 4: call function: parse_file() in lc4_loader.c 	*/
	/*   TODO: call function & check for errors		*/
	/* step 5: repeat steps 3 and 4 for each .OBJ file in argv[] 	*/
	int i=2;
	FILE* read_file = NULL;
	char read_file_name[100];

	while (i < argc){
		sprintf(read_file_name, "%s", argv[i]);
		read_file = NULL;
		read_file = open_file(read_file_name);
		if (read_file == NULL){
			printf("[Read File Open Error] read_file_name : %s \n", read_file_name);
			delete_table(memory);
			return 1;
		}
		pgm_run = parse_file(read_file, memory);
		if (pgm_run != 0){
			printf("[OBJ Parse Error] filename(%d) : %s.\n", i, read_file_name);		
			delete_table(memory);	
			return -1;
		}
		// print_table_lists(memory);	

		/* Close file. Return 2 if error! */
		pgm_run = fclose(read_file);
		if (pgm_run != 0){
			printf("[fclose error] read_file_name : %s\n", read_file_name);	
			delete_table(memory);	
			return 2;
		}
		i++;
	}


	/* step 6: call function: reverse_assemble() in lc4_disassembler.c */
	/*   TODO: call function & check for errors		*/
	pgm_run = reverse_assemble(memory);
	if (pgm_run != 0){
		printf("[reverse_assemble error]\n");	
		fclose(write_file);
		fclose(asm_write_file);
		delete_table(memory);	
		return 3;
	}


	/* step 7: call function: print out the hashtable to output file */
	/*   TODO: call function 				*/
	// Regular Print Table called
	print_table(memory, write_file);	// Print table on the open file
	print_table(memory, stdout);		// Print table on the screen as well

	// Assembly Writing called
	pgm_run = write_assy_on_file (memory, asm_write_file);
	if (pgm_run != 0){
		printf("[write_assy_on_file error] asm_write_file : %s\n", asm_file_name);
		fclose(write_file);
		fclose(asm_write_file);
		delete_table(memory);
		return 2;
	}
	// pgm_run = write_assy_on_file (memory, stdout);			// Print asm on the screen


	/* Close file. Return 2 if error! */
	// Close the .txt file.
	pgm_run = fclose(write_file);
	if (pgm_run != 0){
		printf("[Write file fclose error] write_file : %s\n", write_file_name);
		fclose(asm_write_file);
		delete_table(memory);
		return 2;
	}
	
	// Close the .asm file.
	pgm_run = fclose(asm_write_file);
	if (pgm_run != 0){
		printf("[ASM Write file fclose error] asm_write_file : %s\n", asm_file_name);
		delete_table(memory);
		return 2;
	}
	

	/* step 8: call function: delete_table() in lc4_hash.c */
	/*   TODO: call function & check for errors		*/
	delete_table(memory);

	/* only return 0 if everything works properly */
	return 0 ;
}
