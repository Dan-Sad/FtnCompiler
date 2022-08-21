#pragma once
#include "lex_analyzer.h"
#include "error_handling.h"

#define LINK_COUNT 15
#define TABLE_SIZE 100
#define VAL_STACK_SIZE 20

enum NODE_IDs
{
	
};

/*
	TREE SECTION
*/

// Tree unit
typedef struct _tl_tree_unit
{
	// The token-lexeme
	tl_unit tl;

	struct _tl_tree_unit* prev;

	int index;
	struct _tl_tree_unit* links[LINK_COUNT];
} tl_tree_unit;

// Tree structure
typedef struct tl_tree
{
	tl_tree_unit* root;
	tl_tree_unit* current;
} tl_tree;

/*
	TABLE SECTION
*/

// Variable table unit
typedef struct vt_unit
{
	char var_name[LEXEME_LEN];
	int var_value;
} vt_unit;

typedef struct variable_table
{
	int index;
	vt_unit var_table[TABLE_SIZE];
} variable_table;

/*
	VALUE STACK
*/

typedef struct valsign_stack
{
	int index_val;
	int index_back_val;
	int index_sign;
	int index_back_sign;

	enum TOKEN_IDs pre_last_sign;
	enum TOKEN_IDs last_sign;
	enum TOKEN_IDs last_operated_sign;

	int values[VAL_STACK_SIZE];
	enum TOKEN_IDs signs[VAL_STACK_SIZE];
} valsign_stack;

// Starts the parser
void start_parse(tl_stack* stack_arg, tl_tree* tree_arg);

/*
	parsing utility functions
*/

int rab_while(tl_stack* stack_arg, tl_tree* tree, tl_unit name_do);

int funcsravni(tl_stack* stack_arg); 

void Main_switch(stack_arg, tree_arg);

int expr(tl_stack* stack_arg, tl_tree* tree_arg, valsign_stack* vs_stack_arg, int* result, int* md_result, variable_table* table_arg);
/*
	tl_tree utility functions
*/

// Adds a node to a tree
void add_node(tl_tree* tree, tl_unit tl_arg);
// Move up by one node
void move_up(tl_tree* tree);
// Move down by one node
void move_down(tl_tree* tree, int branch_num);
// Move up to root
void move_to_root(tl_tree* tree);

/*
	variable_table utility functions
*/

// Initialization
void table_init(variable_table* table_arg);

// Adds variable name(identifier) to a table, when meets it
int add_var_name(variable_table* table_arg, char* var_name);
// Adds value to an existing variable
void add_var_value(variable_table* table_arg, char* name, char* value);

// Gets variables value
int get_var_value(variable_table* table_arg, char* var_name);
//double get_var_dvalue();

// Searches for a variable that already exists in the table by its name
int search_var(variable_table* table_arg, char* name);

/*
	val_stack utility functions
*/

void valsign_stack_init(valsign_stack* val_stack_arg);
void valsign_stack_clear(valsign_stack* val_stack_arg);

// Control for values
void push_val(valsign_stack* val_stack_arg, int value_arg);
void back_push_val(valsign_stack* val_stack_arg, int value_arg);
int pop_val(valsign_stack* val_stack_arg);
int back_pop_val(valsign_stack* val_stack_arg);
int peek_val(valsign_stack* val_stack_arg);

int set_back_val_index(valsign_stack* val_stack_arg);

// Control for signs
void push_sign(valsign_stack* val_stack_arg, enum TOKEN_IDs sign_arg);
void back_push_sign(valsign_stack* val_stack_arg, enum TOJEN_IDs sign_arg);
enum TOKEN_IDs pop_sign(valsign_stack* val_stack_arg);
enum TOKEN_IDs back_pop_sign(valsign_stack* val_stack_arg);
enum TOKEN_IDs peek_sign(valsign_stack* val_stack_arg);

//int is_first_op(valsign_stack* val_stack_arg);