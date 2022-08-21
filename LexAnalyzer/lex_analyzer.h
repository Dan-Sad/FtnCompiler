#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "error_handling.h"

// SPACE/TAB ASCII codes defined
#define SPACE 32
#define TAB 9

// Sizes
#define STACK_SIZE 5000
#define LEXEME_LEN 30

enum TOKEN_IDs
{
	PROGRAM = 0, // "program" key-word
	EOI = 1 , EOL = 2, // End of Input | End of Line '\n'
	LP = 3, RP = 4, // ( | )
	CLN = 5, // :
	INT_NUM = 6, DOUBLE_NUM = 7, // 125 | 1.25
	ADD = 8, MULT = 9, SUB = 10, DIV = 11, // + | * | - | /
	LOGIC_G = 12, LOGIC_L =13, LOGIC_GORE = 14, LOGIC_LORE =15, // > | < | >= | <=
	LOGIC_EQ = 16, LOGIC_NEQ = 17, // == | !=
	ASG_OP = 18, // =
	CTRL_FLOW = 19, WHILE_LOOP = 20, // if | while
	DT_INT = 21, DT_DOUBLE = 22, DT_ASG_OP = 23, IDENT_NAME = 24, // int | double | :: | myVar
	END = 25, DO = 26, THEN = 27, ELSE = 28, EXIT = 29, CM = 30, EMPTY = 31, MASTER = 32, TREE_END = 33 // end | do | then | else | exit | , | ON-DUTY VALUE | MASTER-NODE
};

// Token-lexeme unit structure
typedef struct tl_unit
{
	enum TOKEN_IDs id;
	char lexeme[LEXEME_LEN];
} tl_unit;

// Token-lexeme stack structure
// Saves 2 things at once and makes
// them accessible (tokenid)
typedef struct tl_stack
{
	int index;
	tl_unit stack[STACK_SIZE];
} tl_stack;

/* 
	tl_stack & tl_unit utility functions 
*/

// Init
void stack_init(tl_stack* stack_arg);
void unit_init(tl_unit* unit_arg);

// Stack control
void push(tl_stack* stack_arg, tl_unit tl_arg);
tl_unit pop(tl_stack* stack_arg);
tl_unit peek(tl_stack* stack_arg);
int get_index(tl_stack* stack_arg);

/*
	lexical_analyzer utility functions
*/

// Starts the lexical analysis
void start_lex(FILE* input, tl_stack* stack_arg);

// Reads numbers (int/double) | Fills the "output_str" from the beginning "0"
enum TOKEN_IDs readt_number(char first_ch, FILE* input, char* output_str);
// Reads numbers (int/double) | Fills the "output_str" from "predef"
enum TOKEN_IDs readt_number_i(char first_ch, FILE* input, char* output_str, int predef_index);

// Reads idents | Fills the "output_str" from the beginning "0"
void readt_ident(char first_ch, FILE* input, char* output_str);
// Reads idents | Fills the "output_str" from "predef"
void readt_ident_i(char first_ch, FILE* input, char* output_str, int predef_index);

// Reads "symbols_count" amount of characters from the buffer to the "output_str"
void readt(char first_ch, FILE* input, char* output_str, int symbols_count);
