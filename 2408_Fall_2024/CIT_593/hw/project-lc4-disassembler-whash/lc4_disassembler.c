/************************************************************************/
/* File Name : lc4_disassembler.c 										*/
/* Purpose   : This file implements the reverse assembler 				*/
/*             for LC4 assembly.  It will be called by main()			*/
/*             															*/
/* Author(s) : tjf and you												*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lc4_hash.h"
#include "lc4_memory.h"

int imm_converter(int val, int length){
	int unit = 1 << (length-1);		// Get the MSB
	if (unit & val){				// If val's MSB is one (i.e. negative)
		val -= unit * 2;			// subtract min val twice!
	}
	return val;
}

char* create_branch_assy(int subop, int other, unsigned short int curr_address, row_of_memory* bucket_head){
	// printf("[create_branch_assy] subop : %d, other : %d\n", subop, other);
	char* assembly_str = NULL;
	row_of_memory* label_node = NULL;
	unsigned short int label_address = curr_address;	// Label address start from current address.

	if (subop == 0){	// Simple NOP case
		assembly_str = malloc(sizeof(char) * (3 + 1));
		if (assembly_str == NULL){
			printf("[create_branch_assy Error] Memory allocation error.\n");
			return NULL;
		}
		sprintf(assembly_str, "NOP");
	} else{		
		char nzp_str[10];
		char imm9_str[100];
		int nzp_cnt = 0;

		/* Generate temporary nzp string. */
		if (subop & 4){	// 100 Negative!
			nzp_str[nzp_cnt++] = 'n';
		}
		if (subop & 2){	// 010 Zero!
			nzp_str[nzp_cnt++] = 'z';
		}
		if (subop & 1){	// 001 Positive!
			nzp_str[nzp_cnt++] = 'p';
		}
		nzp_str[nzp_cnt] = '\0';

		/* Generate IMM9 label value. */
		label_address += imm_converter(other, 9) + 1;				// Label address adjusted from current address.
		label_node = search_address(bucket_head, label_address);	// Find the node
		if (label_node == NULL){
			printf("[create_branch_assy Error] Label search failed.\n");
			sprintf(imm9_str, " %d", imm_converter(other, 9));
			return NULL;
		} else if (label_node->label == NULL){
			printf("[create_branch_assy Error] Label not exists.\n");
			sprintf(imm9_str, " %d", imm_converter(other, 9));
			return NULL;
		} 

		// Label engraved in the instruction!
		sprintf(imm9_str, " %s", label_node->label);
		

		/* Generate the whole BRnzp Assebmly. */
		assembly_str = malloc(sizeof(char) * (2 + strlen(nzp_str) + strlen(imm9_str) + 1));
		if (assembly_str == NULL){
			printf("[create_branch_assy Error] Memory allocation error.\n");
			return NULL;
		}
		sprintf(assembly_str, "BR");
		strcat(assembly_str, nzp_str);
		strcat(assembly_str, imm9_str);
	}

	return assembly_str;
}

char* create_arithmetic_assy(int subop, int rd, int rs, int rt, int other){
	// printf("[create_arithmetic_assy] rd : %d, rs : %d, subop : %d, rt : %d, other : %d\n", rd, rs, subop, rt, other);
	char* assembly_str = NULL;
	char common_str[20];
	sprintf(common_str, " R%d, R%d, ", rd, rs);
	
	if (subop < 4){
		char rt_str[10];
		sprintf(rt_str, "R%d", rt);
		assembly_str = malloc(sizeof(char)*(3 + strlen(common_str) + strlen(rt_str) + 1));
		if (assembly_str == NULL){
			printf("[create_arithmetic_assy Error] Memory allocation error.\n");
			return NULL;
		}

		if (subop == 0){					// 000 ADD
			sprintf(assembly_str, "ADD");
		} else if (subop == 1){				// 001 MUL
			sprintf(assembly_str, "MUL");
		} else if (subop == 2){				// 010 SUB
			sprintf(assembly_str, "SUB");
		} else{								// 011 DIV
			sprintf(assembly_str, "DIV");	
		}

		strcat(assembly_str, common_str);
		strcat(assembly_str, rt_str);

	} else{		// ADD IMM5
		// printf("short int size : %zu\n", sizeof(short int));	// short int size : 2
		char imm5_str[5];		// -16 <= imm5 < 16
		sprintf(imm5_str, "#%d", imm_converter(other, 5));
		assembly_str = malloc(sizeof(char)*(3 + strlen(common_str) + strlen(imm5_str) + 1));
		if (assembly_str == NULL){
			printf("[create_arithmetic_assy Error] Memory allocation error.\n");
			return NULL;
		}
		sprintf(assembly_str, "ADD");

		strcat(assembly_str, common_str);		
		strcat(assembly_str, imm5_str);
	}

	return assembly_str;
}

char* create_comparison_assy(int subop, int rs, int rt, int other){
	// printf("[create_comparison_assy] subop : %d, rs : %d, rt : %d, other : %d\n", subop, rs, rt, other);
	char* assembly_str = NULL;
	char str_by_subop[20];
	
	switch (subop){
		case 0:		// CMP
			sprintf(str_by_subop, " R%d, R%d", rs, rt);
			break;
		case 1:		// CMPU
			sprintf(str_by_subop, "U R%d, R%d", rs, rt);
			break;
		case 2:		// CMPI
			sprintf(str_by_subop, "I R%d, #%d", rs, imm_converter(other, 7));
			break;
		case 3:		// CMPIU
			sprintf(str_by_subop, "IU R%d, #%d", rs, other);
	}

	assembly_str = malloc(sizeof(char) * (3 + strlen(str_by_subop) + 1));
	if (assembly_str == NULL){
		printf("[create_comparison_assy Error] Memory allocation error.\n");
		return NULL;
	}
	sprintf(assembly_str, "CMP");
	strcat(assembly_str, str_by_subop);

	return assembly_str;
}

char* create_jsr_assy(char* opcode_str, int subop, int rs, int other, int is_os, row_of_memory* bucket_head){
	char* assembly_str = NULL;
	char str_by_subop[100];
	row_of_memory* label_node = NULL;
	unsigned short int label_address = 0x0000;	// Label address initialized to User Code starting address.
	if (is_os) label_address = 0x8000;			// Update if it's OS Code starting address.
	
	if (subop == 1){	// JSR
		label_address += imm_converter(other, 11) * 0x10;			// Multiple of 16!
		label_node = search_address(bucket_head, label_address);
		if (label_node == NULL){
			printf("[create_jsr_assy Error] Label search failed.\n");
			sprintf(str_by_subop, " %d", imm_converter(other, 11));
			// return NULL;
		} else if (label_node->label == NULL){
			printf("[create_jsr_assy Error] Label not exists.\n");
			sprintf(str_by_subop, " %d", imm_converter(other, 11));
			// return NULL;
		} else{
			sprintf(str_by_subop, " %s", label_node->label);
		}

	} else{				// JSRR
		sprintf(str_by_subop, "R R%d", rs);
	}

	assembly_str = malloc(sizeof(char) * (strlen(opcode_str) + strlen(str_by_subop) + 1));
	if (assembly_str == NULL){
		printf("[create_jsr_assy Error] Memory allocation error.\n");
		return NULL;
	}
	sprintf(assembly_str, "");
	strcat(assembly_str, opcode_str);
	strcat(assembly_str, str_by_subop);

	return assembly_str;\
}


char* create_jmp_assy(char* opcode_str, int subop, int rs, int other, unsigned short int curr_address, row_of_memory* bucket_head){
	// printf("[create_jmp_assy] subop : %d, rs : %d, other : %d\n", subop, rs, other);
	char* assembly_str = NULL;
	char str_by_subop[100];
	row_of_memory* label_node = NULL;
	unsigned short int label_address = curr_address;	// Start from curr_node's address
	
	if (subop == 1){	// JMP
		label_address += imm_converter(other, 11) + 1;	// Label address adjusted from current address by IMM11
		label_node = search_address(bucket_head, label_address);
		if (label_node == NULL){
			printf("[create_jmp_assy Error] Label search failed.\n");
			sprintf(str_by_subop, " %d", imm_converter(other, 11));
			return NULL;
		} else if (label_node->label == NULL){
			printf("[create_jmp_assy Error] Label not exists.\n");
			sprintf(str_by_subop, " %d", imm_converter(other, 11));
			return NULL;
		} 
			
		sprintf(str_by_subop, " %s", label_node->label);

	} else{				// JMPR
		sprintf(str_by_subop, "R R%d", rs);
	}

	assembly_str = malloc(sizeof(char) * (strlen(opcode_str) + strlen(str_by_subop) + 1));
	if (assembly_str == NULL){
		printf("[create_jmp_assy Error] Memory allocation error.\n");
		return NULL;
	}
	sprintf(assembly_str, "");
	strcat(assembly_str, opcode_str);
	strcat(assembly_str, str_by_subop);

	return assembly_str;
}

char* create_logical_assy(int subop, int rd, int rs, int rt, int other){
	// printf("[create_logical_assy] rd : %d, rs : %d, subop : %d, rt : %d, other : %d\n", rd, rs, subop, rt, other);
	char* assembly_str = NULL;
	char common_str[20];
	sprintf(common_str, " R%d, R%d, ", rd, rs);
	
	if (subop == 1){	// 001 NOT
		common_str[7] = '\0';	// Trim chars after rd.
		assembly_str = malloc(sizeof(char)*(3 + strlen(common_str) + 1));
		if (assembly_str == NULL){
			printf("[create_logical_assy Error] Memory allocation error.\n");
			return NULL;
		}
		sprintf(assembly_str, "NOT");
		strcat(assembly_str, common_str);
	} else if (subop < 4){
		char rt_str[10];
		sprintf(rt_str, "R%d", rt);
		assembly_str = malloc(sizeof(char)*(3 + strlen(common_str) + strlen(rt_str) + 1));
		if (assembly_str == NULL){
			printf("[create_logical_assy Error] Memory allocation error.\n");
			return NULL;
		}

		if (subop == 0){					// 000 AND
			sprintf(assembly_str, "AND");
		} else if (subop == 2){				// 010 OR
			sprintf(assembly_str, "OR");
		} else{								// 011 XOR
			sprintf(assembly_str, "XOR");	
		}

		strcat(assembly_str, common_str);
		strcat(assembly_str, rt_str);
	} else{		// AND IMM5
		char imm5_str[10];
		sprintf(imm5_str, "#%d", imm_converter(other, 5));
		assembly_str = malloc(sizeof(char)*(3 + strlen(common_str) + strlen(imm5_str) + 1));
		if (assembly_str == NULL){
			printf("[create_logical_assy Error] Memory allocation error.\n");
			return NULL;
		}
		sprintf(assembly_str, "AND");

		strcat(assembly_str, common_str);		
		strcat(assembly_str, imm5_str);
	}

	return assembly_str;
}

char* create_load_store_assy(char* optcode_str, int rd, int rs, int other){
	// printf("[create_load_store_assy] rd : %d, rs : %d, other : %d\n", rd, rs, other);
	char* assembly_str = NULL;
	char arg_str[20];
	sprintf(arg_str, " R%d, R%d, #%d", rd, rs, imm_converter(other, 6));

	assembly_str = malloc(sizeof(char) * (strlen(optcode_str) + strlen(arg_str) + 1));
	if (assembly_str == NULL){
		printf("[create_load_store_assy Error] Memory allocation error.\n");
		return NULL;
	}
	sprintf(assembly_str, "");
	strcat(assembly_str, optcode_str);
	strcat(assembly_str, arg_str);

	return assembly_str;
}

char* create_const_assy(int rd, int other){
	// printf("[create_const_assy] rd : %d, other : %d\n", rd, other);
	char* assembly_str = NULL;
	char arg_str[20];	
	sprintf(arg_str, " R%d, #%d", rd, imm_converter(other, 9));

	assembly_str = malloc(sizeof(char) * (5 + strlen(arg_str) + 1));
	if (assembly_str == NULL){
		printf("[create_const_assy Error] Memory allocation error.\n");
		return NULL;
	}
	sprintf(assembly_str, "CONST");
	strcat(assembly_str, arg_str);

	return assembly_str;
}

char* create_shift_assy(int subop, int rd, int rs, int rt, int other){
	// printf("[create_shift_assy] rd : %d, rs : %d, subop : %d, rt : %d, other : %d\n", rd, rs, subop, rt, other);
	char* assembly_str = NULL;
	char common_str[20];
	sprintf(common_str, " R%d, R%d, ", rd, rs);
	char operator_str[4];
	char subop_arg_str[20];
	
	if (subop == 3){	// 11 MOD
		sprintf(operator_str, "MOD");
		sprintf(subop_arg_str, "R%d", rt);
	} else {
		if (subop == 0){		// 00 SLL
			sprintf(operator_str, "SLL");
		} else if (subop == 1){	// 01 SRA
			sprintf(operator_str, "SRA");
		} else{					// 00 SRL
			sprintf(operator_str, "SRL");
		} 
		sprintf(subop_arg_str, "#%d", other);
	} 

	assembly_str = malloc(sizeof(char)*(strlen(operator_str) + strlen(common_str) + strlen(subop_arg_str) + 1));
	if (assembly_str == NULL){
		printf("[create_shift_assy Error] Memory allocation error.\n");
		return NULL;
	}
	sprintf(assembly_str, "");
	strcat(assembly_str, operator_str);
	strcat(assembly_str, common_str);
	strcat(assembly_str, subop_arg_str);

	return assembly_str;
}

char* create_hiconst_assy(int rd, int other){
	// printf("[create_hiconst_assy] rd : %d, other : %d\n", rd, other);
	char* assembly_str = NULL;
	char arg_str[20];	
	sprintf(arg_str, " R%d, #%d", rd, other);

	assembly_str = malloc(sizeof(char) * (7 + strlen(arg_str) + 1));
	if (assembly_str == NULL){
		printf("[create_hiconst_assy Error] Memory allocation error.\n");
		return NULL;
	}
	sprintf(assembly_str, "HICONST");
	strcat(assembly_str, arg_str);

	return assembly_str;
}

char* create_trap_assy(int other){
	// printf("[create_trap_assy] other : %d\n", other);
	char* assembly_str = NULL;
	char arg_str[20];	
	sprintf(arg_str, " #%d", other);

	assembly_str = malloc(sizeof(char) * (4 + strlen(arg_str) + 1));
	if (assembly_str == NULL){
		printf("[create_trap_assy Error] Memory allocation error.\n");
		return NULL;
	}
	sprintf(assembly_str, "TRAP");
	strcat(assembly_str, arg_str);

	return assembly_str;
}

int assembly_str_generator(row_of_memory* bucket_head, row_of_memory* node, int opcode, int is_os){
	char* assembly_str = NULL;
	int str_len = 1;
	unsigned short int mask;
	int rd;
	int rs;
	int subop;
	int rt;
	int other;

	switch(opcode){
		case 0:		// 0001	Branch
			/* Get assembly components with masking and shifting. */
			subop = (node->contents & 0x0e00) >> 9;
			other = node->contents & 0x01ff;

			assembly_str = create_branch_assy(subop, other, node->address, bucket_head);
			if (assembly_str == NULL){
				return 1;
			}
			break;
		case 1:		// 0001	Arithmetic
			/* Get assembly components with masking and shifting. */
			rd = (node->contents & 0x0e00) >> 9;
			rs = (node->contents & 0x01c0) >> 6;
			subop = (node->contents & 0x0038) >> 3;
			rt = (node->contents & 0x0007);
			other = (node->contents & 0x001f);

			assembly_str = create_arithmetic_assy(subop, rd, rs, rt, other);
			if (assembly_str == NULL){
				return 1;
			}
			break;
		case 2:		// 0010	Comparison
			/* Get assembly components with masking and shifting. */
			rs = (node->contents & 0x0e00) >> 9;
			subop = (node->contents & 0x0180) >> 7;
			rt = (node->contents & 0x0007);
			other = (node->contents & 0x007f);
			
			assembly_str = create_comparison_assy(subop, rs, rt, other);
			if (assembly_str == NULL){
				return 1;
			}
			break;
		case 4:		// 0100	Jump to Subroutine
			// char opcode_str[] = "JSR";
			subop = (node->contents & 0x0800) >> 11;
			rs = (node->contents & 0x01c0) >> 6;
			other = (node->contents & 0x07ff);

			assembly_str = create_jsr_assy("JSR", subop, rs, other, is_os, bucket_head);
			if (assembly_str == NULL){
				return 1;
			}
			break;
		case 5:		// 0101	Logical
			/* Get assembly components with masking and shifting. */
			rd = (node->contents & 0x0e00) >> 9;
			rs = (node->contents & 0x01c0) >> 6;
			subop = (node->contents & 0x0038) >> 3;
			rt = (node->contents & 0x0007);
			other = (node->contents & 0x001f);

			assembly_str = create_logical_assy(subop, rd, rs, rt, other);
			if (assembly_str == NULL){
				return 1;
			}
			break;
		case 6:		// 0110	Load
		case 7:		// 0111	Store
			/* Get assembly components with masking and shifting. */
			rd = (node->contents & 0x0e00) >> 9;
			rs = (node->contents & 0x01c0) >> 6;
			other = (node->contents & 0x003f);

			if (opcode == 6){	// LDR
				assembly_str = create_load_store_assy("LDR", rd, rs, other);
			} else{				// STR
				assembly_str = create_load_store_assy("STR", rd, rs, other);
			}

			if (assembly_str == NULL){
				return 1;
			}
			break;
		case 8:		// 1000 RTI
			/* Directly assign "RTI". */
			assembly_str = malloc(sizeof(char) * (3 + 1));
			if (assembly_str == NULL){
				printf("[assembly_str_generator Error] Opcode RTI, memory allocation.\n");
				return 1;
			}
			sprintf(assembly_str, "RTI");
			break;
		case 9:		// 1001 CONST
			/* Get assembly components with masking and shifting. */
			rd = (node->contents & 0x0e00) >> 9;
			other = (node->contents & 0x01ff);

			assembly_str = create_const_assy(rd, other);
			if (assembly_str == NULL){
				return 1;
			}
			break;
		case 10:	// 1010	Shift
			/* Get assembly components with masking and shifting. */
			rd = (node->contents & 0x0e00) >> 9;
			rs = (node->contents & 0x01c0) >> 6;
			subop = (node->contents & 0x0030) >> 4;
			rt = (node->contents & 0x0007);
			other = (node->contents & 0x000f);

			assembly_str = create_shift_assy(subop, rd, rs, rt, other);
			if (assembly_str == NULL){
				return 1;
			}
			break;
		case 12:	// 1100	Jump
			// char opcode_str[] = "JMP";
			subop = (node->contents & 0x0800) >> 11;
			rs = (node->contents & 0x01c0) >> 6;
			other = (node->contents & 0x07ff);

			assembly_str = create_jmp_assy("JMP", subop, rs, other, node->address, bucket_head);
			if (assembly_str == NULL){
				return 1;
			}
			break;
		case 13:	// 1101	Hiconst
			/* Get assembly components with masking and shifting. */
			subop = (node->contents & 0x0100);
			if (subop == 0){
				printf("[Error] Unexpected subopcode. Content : %04x\n", node->contents);
				return -1;
			}
			rd = (node->contents & 0x0e00) >> 9;
			other = node->contents & 0x00ff;

			assembly_str = create_hiconst_assy(rd, other);
			if (assembly_str == NULL){
				return 1;
			}
			break;
		case 15:	// 1111	Trap
			other = node->contents & 0x00ff;

			assembly_str = create_trap_assy(other);
			if (assembly_str == NULL){
				return 1;
			}
			break;
		default:
			printf("[Error] Unexpected subopcode. Content : %04x\n", node->contents);
			return -2;
	}

	if (assembly_str == NULL){
		char default_assy_str[] = "[Assy not implemented]";
		assembly_str = malloc(strlen(default_assy_str)+1);
		strcpy(assembly_str, default_assy_str);
	}

	node->assembly = assembly_str;	// Assign the newly generated assembly to node.
	return 0;
}

char* data_blkw_str_generator(int blkw_cnt){
	char* blkw_str = NULL;
	char blkw_directive[] = ".BLKW ";

	char blkw_arg_str[20];		
	sprintf(blkw_arg_str, "x%x", blkw_cnt);

	blkw_str = malloc(sizeof(char) * (strlen(blkw_directive) + strlen(blkw_arg_str) + 1));
	if (blkw_str == NULL){
		printf("[data_blkw_str_generator Error] Malloc failure.\n");
		return NULL;		// Return NULL if error takes place;
	}
	sprintf(blkw_str, "");
	strcat(blkw_str, blkw_directive);
	strcat(blkw_str, blkw_arg_str);

	return blkw_str;
}

char* data_fill_str_generator(unsigned short int contents){
	char* data_str = NULL;
	char data_val[10];
	sprintf(data_val, " #%d", imm_converter(contents, 16));

	data_str = malloc(sizeof(char) * (5 + strlen(data_val) + 1));
	if (data_str == NULL){
		printf("[data_fill_str_generator Error] Malloc failure.\n");
		return NULL;		// Return NULL if error takes place;
	}
	sprintf(data_str, ".FILL");
	strcat(data_str, data_val);

	return data_str;
}

int reverse_assemble (lc4_memory_segmented* memory) 
{	
	int opcode;
	row_of_memory* bucket_head = NULL;
	row_of_memory* curr_node = NULL;
	int pgm_run = 0;
	int bucket_idx = 0;
	int blkw_cnt = 0;
	row_of_memory* blkw_start_node = NULL;
	char* assy_str = NULL;

	while (bucket_idx < memory->num_of_buckets){
		/* Skip if bucket is empty. */
		bucket_head = memory->buckets[bucket_idx];
		if (bucket_head == NULL){	
			bucket_idx++;							// skip it.
			continue;
		}

		if (bucket_idx % 2 == 0){	// User Code, OS Code
		// if (1){	// User Code, OS Code
			opcode = 0;
			while (opcode < 16){		
				curr_node = bucket_head;		// Start from each bucket's head.
				if (opcode == 3 || opcode == 11 || opcode == 14){
					opcode++;
					continue;	// Skip undefined opcodes 0011, 1011, 1110
					printf("[reverse_assemble Error] Undefined opcode. ");
					printf("content : %04x, opcode : %d\n", curr_node->contents, opcode);
					return 1;		// Return 1 if error takes place;
				}
				while (1){
					curr_node = search_opcode(curr_node, opcode);	// Use prev search result as the starting point.
					if (curr_node == NULL) break;					// Escape if nothing was found.

					// printf("bucket : %d, content : %04x, opcode : %d\n", bucket_idx, curr_node->contents, opcode);
					pgm_run = assembly_str_generator(bucket_head, curr_node, opcode, bucket_idx/2);
					if (pgm_run != 0){
						printf("[Reverse Assemble Error] User/OS Code bucket : %d, ", bucket_idx);
						printf("content : %04x, opcode : %d\n", curr_node->contents, opcode);
						return 1;		// Return 1 if error takes place;
					}
				}
				opcode++;
			}
		} else {	// User Data, OS Data
			curr_node = bucket_head;

			while (curr_node != NULL){
				assy_str = NULL;
				// If content == 0, .BLKW starts.
				if (curr_node->contents == 0){
					if (blkw_start_node == NULL){
						blkw_start_node = curr_node;
					}
					blkw_cnt++;

				// Otherwise, check if the previous 0-content-lines exists and deal with it. And .FILL the current.
				} else{

					// Previous 0-content-lines treatment logic starts.
					if (blkw_cnt > 0){
						// If only one 0 content, .FILL #0
						if (blkw_cnt == 1){
							assy_str = data_fill_str_generator(blkw_start_node->contents);
							if (assy_str == NULL){
								printf("[reverse_assemble Error] One .BLKW to .FILL assy not generated.");
								printf("(blkw_start_node addr : %04x\n", blkw_start_node->address);
								return 1;
							}

						// If multiple 0 contents, .BLKW at the beginning of the 0-contents.
						} else if (blkw_cnt > 1) {
							assy_str = data_blkw_str_generator(blkw_cnt);
							if (assy_str == NULL){
								printf("[reverse_assemble Error] BLKW assy not generated.");
								printf("(curr_node addr : %04x", curr_node->address);
								printf("/blk_start_node addr : %04x)\n", blkw_start_node->address);
								return 1;
							}
						}
						blkw_start_node->assembly = assy_str;
						blkw_start_node = NULL;
						blkw_cnt = 0;
					}
					// Previous 0-content-lines treatment logic ends.
					
					// .FILL for the curr_node.
					assy_str = data_fill_str_generator(curr_node->contents);
					if (assy_str == NULL){
						printf("[reverse_assemble Error] .FILL assy not generated.");
						printf("(curr_node addr : %04x\n", curr_node->address);
						return 1;
					}
					curr_node->assembly = assy_str;
					
				}
				curr_node = curr_node->next;
			}

			// If one 0 content, .FILL #0
			if (blkw_cnt == 1){
				assy_str = data_fill_str_generator(blkw_start_node->contents);
				if (assy_str == NULL){
					printf("[reverse_assemble Error] (End) One .BLKW to .FILL assy not generated.");
					printf("(blkw_start_node addr : %04x\n", blkw_start_node->address);
					return 1;
				}
			// If multiple 0 contents, .BLKW
			} else if (blkw_cnt > 1) {
				assy_str = data_blkw_str_generator(blkw_cnt);
				if (assy_str == NULL){
					printf("[reverse_assemble Error] (End) BLKW assy not generated.");
					printf("(blk_start_node addr : %04x)\n", blkw_start_node->address);
					return 1;
				}
			}
			blkw_start_node->assembly = assy_str;
			blkw_start_node = NULL;
			blkw_cnt = 0;
		}

		bucket_idx++;
	}

	return 0 ;
}
