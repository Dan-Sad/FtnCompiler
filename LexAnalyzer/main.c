#include "error_handling.h"
#include "lex_analyzer.h"
#include "parser_new.h"
#include "code_generator.h"

FILE* input = NULL;

tl_stack toklex_stack;
tl_stack toklex_stack_inv;

tl_tree myTree;

int main(int argc, char** argv)
{
	tl_unit master_node = {MASTER, ""};
	add_node(&myTree, master_node);

	stack_init(&toklex_stack);
	stack_init(&toklex_stack_inv);

	/*
	// Tree node setup
	// Adding master-node
	tl_unit master_node = {MASTER, ""};

	add_node(&myTree, master_node);*/

	if (argc > 1)
	{
		input = fopen(argv[1], "r");
		if (input == NULL)
		{
			printf("File could not be opened/created!\n");
			return -1;
		}
	}
	else
	{
		char* inputfile_name = (char*)malloc(50 * sizeof(char));

		//printf("Please, enter the input file name: ");
		//scanf("%s", inputfile_name);

		input = fopen("inputdata.txt", "r"); // !!!!
		if (input == NULL)
		{
			printf("File could not be opened/created!\n");
			free(inputfile_name);
			return -1;
		}

		free(inputfile_name);
	}
	
	// By the end of this function toklex_stack
	// will be full of token-lexeme elements
	// and ready for passing to the Parser
	start_lex(input, &toklex_stack);

	// Closing an input file stream
	fclose(input);

	// Pushing the EOI element to the bottom of the stack
	// and inverting current "toklex_stack" to a proper
	// order form in "toklex_stack_inv"
	tl_unit current_tl;
	char* tmp_lexeme_string = (char*)malloc(LEXEME_LEN);
	if (tmp_lexeme_string == NULL)
	{
		printf("Could not allocate memory for temporary string!\n");
		return -3;
	}

	current_tl.id = EOI;
	tmp_lexeme_string[0] = '\0';
	strcpy(current_tl.lexeme, tmp_lexeme_string);
	push(&toklex_stack_inv, current_tl);

	free(tmp_lexeme_string);

	int element_count = 0;
	while (element_count < STACK_SIZE && peek(&toklex_stack).id != EOI)
	{
		push(&toklex_stack_inv, pop(&toklex_stack));
		element_count++;
	}

	// Starting a parser
	start_parse(&toklex_stack_inv, &myTree);

	// Starting code generator
	start_codegen(&myTree);

	return 0;
}