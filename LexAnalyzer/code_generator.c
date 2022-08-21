#include "code_generator.h"

FILE* output = NULL;
variable_table cg_vtable;

void start_codegen(tl_tree* tree_arg)
{
	table_init(&cg_vtable);

	// Output file preparation section
	char* outputfile_name = (char*)malloc(50 * sizeof(char));

	//printf("Please, enter the output file name: ");///////////////////////////////////////////////////////////////////////////////////////////////////
	//scanf("%s", outputfile_name);///////////////////////////////////////////////////////////////////////////////////////////////////

	output = fopen("outputdata.txt", "w");
	if (output == NULL)
	{
		printf("File could not be opened/created!\n");
		free(outputfile_name);
		return;
	}

	free(outputfile_name);

	// Code generation section
	move_to_root(tree_arg);

	// Creating an output stream
	char* output_stream = (char*)malloc(OUTPUT_STREAM_LEN);
	if (output_stream == NULL)
	{
		printf("Output stream was not created!\n");
		return;
	}
	
	char* help_string = (char*)malloc(OUTPUT_STREAM_LEN);
	char* var_name_string = (char*)malloc(OUTPUT_STREAM_LEN);

	int var = 0;
	int term = 0;

	int section_number = 0;
	while (section_number < tree_arg->root->index)
	{
		move_down(tree_arg, section_number);

		switch (tree_arg->root->links[section_number]->tl.id)
		{
		case IDENT_NAME:
			strcpy(var_name_string, tree_arg->current->tl.lexeme);

			move_down(tree_arg, 0);

			if (tree_arg->current->tl.id == ASG_OP)
			{
				move_down(tree_arg, 0);

				while (tree_arg->current->tl.id != EMPTY)
				{
					strcpy(output_stream, "mov eax, ");// OUTPUT "mov eax, "
					switch (tree_arg->current->tl.id)
					{
					case INT_NUM:
						// mov eax, 0-9
						strcat(output_stream, tree_arg->current->tl.lexeme);
						strcat(output_stream, "\n"); 

						add_var_value(&cg_vtable, var_name_string, tree_arg->current->tl.lexeme);

						fprintf(output, output_stream);
						output_stream[0] = '\0';
						break;
					case IDENT_NAME:
						// get value
						// OUTPUT
						// int :: x, y
						// x = 0
						// x = x + 3
						// y = x * 2

						// mov eax, 0
						// mov eax, 3
						// mov ebx, 3
						// add eax, ebx

						_itoa(get_var_value(&cg_vtable, tree_arg->current->tl.lexeme), help_string, 10);
						strcat(output_stream, help_string);
						strcat(output_stream, "\n");

						add_var_value(&cg_vtable, var_name_string, help_string);

						fprintf(output, output_stream);
						output_stream[0] = '\0';
						break;

					case ADD:
					case SUB:
					case MULT:
					case DIV:
					{
						enum TOKEN_IDs sign = tree_arg->current->tl.id;

						move_down(tree_arg, 0);

						strcpy(output_stream, "mov ebx, ");// OUTPUT "mov ebx, "
						switch (tree_arg->current->tl.id)
						{
						case INT_NUM:
							// mov eax, 0-9
							strcat(output_stream, tree_arg->current->tl.lexeme);
							strcat(output_stream, "\n");

							add_var_value(&cg_vtable, var_name_string, tree_arg->current->tl.lexeme);

							fprintf(output, output_stream);
							output_stream[0] = '\0';
							break;
						case IDENT_NAME:
							// get value
							// OUTPUT
							// int :: x, y
							// x = 0
							// x = x + 3
							// y = x * 2

							// mov eax, 0
							// mov eax, 3
							// mov ebx, 3
							// add eax, ebx

							_itoa(get_var_value(&cg_vtable, tree_arg->current->tl.lexeme), help_string, 10);
							strcat(output_stream, help_string);
							strcat(output_stream, "\n");

							//add_var_value(&cg_vtable, var_name_string, help_string);

							fprintf(output, output_stream);
							output_stream[0] = '\0';

						}
						switch (sign)
						{
						case ADD:
							strcpy(output_stream, "add eax, ebx\n");

							var = get_var_value(&cg_vtable, var_name_string);
							term = atoi(help_string);
							_itoa(var + term, help_string, 10);

							add_var_value(&cg_vtable, var_name_string, help_string);

							fprintf(output, output_stream);
							output_stream[0] = '\0';
							break;
						case SUB:
							strcpy(output_stream, "sub eax, ebx\n");

							var = get_var_value(&cg_vtable, var_name_string);
							term = atoi(help_string);
							_itoa(var - term, help_string, 10);

							add_var_value(&cg_vtable, var_name_string, help_string);

							fprintf(output, output_stream);
							output_stream[0] = '\0';
							break;
						case MULT:
							strcpy(output_stream, "imul eax, ebx\n");

							var = get_var_value(&cg_vtable, var_name_string);
							term = atoi(help_string);
							_itoa(var * term, help_string, 10);

							add_var_value(&cg_vtable, var_name_string, help_string);

							fprintf(output, output_stream);
							output_stream[0] = '\0';
							break;
						case DIV:
							strcpy(output_stream, "idiv eax, ebx\n");

							var = get_var_value(&cg_vtable, var_name_string);
							term = atoi(help_string);
							_itoa(var / term, help_string, 10);

							add_var_value(&cg_vtable, var_name_string, help_string);

							fprintf(output, output_stream);
							output_stream[0] = '\0';
							break;
						}
					}
					default:
						break;
					}

					move_down(tree_arg, 0);
				}
			}
			else
			{
				
				break;
			}
			
			break;
		case DT_INT:
		{
			int init_section_number = 0;
			move_down(tree_arg, init_section_number);
			
			while (tree_arg->current->tl.id != EMPTY)
			{
				add_var_name(&cg_vtable, tree_arg->current->tl.lexeme);

				init_section_number++;
				move_up(tree_arg);
				move_down(tree_arg, init_section_number);
			}

		break;
		}
		default:
			break;
		}

		move_to_root(tree_arg);
		section_number++;
	}

	strcpy(output_stream, "push eax\n");
	fprintf(output, output_stream);
	output_stream[0] = '\0';

	free(output_stream);
	free(help_string);
	free(var_name_string);
	fclose(output);
}