/************************************************************************/
/* File Name : lc4_disassembler.h 										*/
/* Purpose   : This function declares functions for lc4_dissembler.c	*/
/*      																*/
/*             															*/
/* Author(s) : tjf 														*/
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"
#include "lc4_hash.h"

/*
 * declarations of functions that must defined in lc4_disassembler.c
 */

/**
 * Converts the UIMM value <val> into given length IMM-<length> value. 
*/
int imm_converter(int val, int length);

/**
 * Functions that create the assembly instruction for each opcode-subopcode combinations. 
*/
char* create_branch_assy(int subop, int other, unsigned short int curr_address, row_of_memory* bucket_head);	// Branch Instructions
char* create_arithmetic_assy(int subop, int rd, int rs, int rt, int other); // Arithmetic Instructions
char* create_comparison_assy(int subop, int rs, int rt, int other);	// Comparison Instructions
char* create_jsr_assy(char* opcode_str, int subop, int rs, int other, int is_os, row_of_memory* bucket_head); // JSR Instructions
char* create_jmp_assy(char* opcode_str, int subop, int rs, int other, unsigned short int curr_address, row_of_memory* bucket_head);	// Jump Instructions
char* create_logical_assy(int subop, int rd, int rs, int rt, int other);	// Logical Instructions
char* create_load_store_assy(char* optcode_str, int rd, int rs, int other);	// Load & Store Instructions
char* create_const_assy(int rd, int other);	// Const Instruction
char* create_shift_assy(int subop, int rd, int rs, int rt, int other);	// Shift Instructions
char* create_hiconst_assy(int rd, int other);	// Hiconst Instruction
char* create_trap_assy(int other);	// Trap Instruction
int assembly_str_generator(row_of_memory* bucket_head, row_of_memory* node, int opcode, int is_os); // Generates assemblies by calling functions above.

char* data_blkw_str_generator(int blkw_cnt);				// Generate .BLKW directive for .DATA
char* data_fill_str_generator(unsigned short int contents);	// Generate .FILL directive for .DATA

/**
 * translates the hex representation of arithmetic instructions
 * into their assembly equivalent in the right buckets of the hashtable.
 *
 * returns 0 upon success, and non-zero if an error occurs.
 */
int reverse_assemble (lc4_memory_segmented* memory) ;

