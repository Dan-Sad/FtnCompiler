#include "parser_new.h"

variable_table var_table;
valsign_stack vs_stack;

tl_unit name_do;
int wkey = 0;
int index_while = 0;
int supkey = -1;
tl_unit prog_name;

void start_parse(tl_stack* stack_arg, tl_tree* tree_arg)
{
	table_init(&var_table);
	valsign_stack_init(&vs_stack);

	if (pop(stack_arg).id == PROGRAM)
	{
		if (peek(stack_arg).id == IDENT_NAME)
		{
			prog_name = pop(stack_arg);
			// Main loop
			while (1)
			{
				if (peek(stack_arg).id == EOI)
				{
					stack_arg->index = get_index(stack_arg) + 3;
					if (pop(stack_arg).id != END || pop(stack_arg).id != PROGRAM || strcmp(pop(stack_arg).lexeme, prog_name.lexeme) != 0)
						puts("haven't 'END PROGRAM name'");
					break;
				}
				Main_switch(stack_arg, tree_arg);
			}
		}
		else
		{
			missing_token_err("IDENT_NAME");
		}
	}
	else
	{
		missing_token_err("PROGRAM");
	}
}
void Main_switch(tl_stack * stack_arg, tl_tree * tree_arg)
{
	tl_unit current_tl = pop(stack_arg);
	tl_unit lookahead;

	switch (current_tl.id)
	{
	case CTRL_FLOW:
	{
		move_to_root(tree_arg);

		char* str = (char*)malloc(30 * sizeof(char));
		int id, oldid = 0, ikey;
		ikey = 1;
		if (pop(stack_arg).id == LP)//'LP'	'('
		{
			if ((id = peek(stack_arg).id) == INT_NUM)//'INT_NUM'(1/0)
			{
				if (strcmp(pop(stack_arg).lexeme, "0") == 0)
				{
					if (pop(stack_arg).id == RP) //'RP'		')'
					{
						id = pop(stack_arg).id;
						while (id != ELSE)// 'поиск ELSE'
						{
							if ((oldid == END) && (id == CTRL_FLOW)) // 'проверяем на END IF'
							{
								break;
							}
							if (id == EOI)
							{
								puts("not have END IF");
								return;
							}
							oldid = id;
							id = pop(stack_arg).id;
						}
						if (id == ELSE)// 'ELSE при int 0'
						{
							tl_unit lexemka;
							while (1)
							{
								lexemka = peek(stack_arg);
								if (((id = pop(stack_arg).id) == END) && (peek(stack_arg).id == CTRL_FLOW)) // 'работаем до END IF'
								{
									pop(stack_arg).id;
									break;
								}
								if (id == EOI)
								{
									puts("not have END IF");
									return;
								}
								if (id == EXIT)
									if (strcmp(peek(stack_arg).lexeme, name_do.lexeme) == 0)
									{
										supkey = 0;
										wkey = 555;
										break;
									}
								push(stack_arg, lexemka);
								Main_switch(stack_arg, tree_arg);
							}
						}
					}
					else
					{
						puts("not have ')'");
						return 1;
					}
				}
				else
				{
					if (pop(stack_arg).id == RP) //'RP'(')')
					{
						while (1)// 'гоним до END If если нет then'
						{
							id = pop(stack_arg).id;
							if (id == THEN)
							{
								break;
							}
							if ((id == END) && (peek(stack_arg).id == CTRL_FLOW)) // 'END IF'
							{
								pop(stack_arg).id;
								break;
							}
							if (id == EOI)
							{
								puts("not have END IF");
								return;
							}
						}
						if (id == THEN)// 'THEN при int 1'
						{
							tl_unit lexemka;
							while (1)// 'END'
							{
								lexemka = peek(stack_arg);
								if (((id = pop(stack_arg).id) == END) && (peek(stack_arg).id == CTRL_FLOW)) // 'прогоняем до END IF'
								{
									pop(stack_arg).id;
									break;
								}
								if (id == EOI)
								{
									puts("not have END IF");
									return;
								}
								if (id == EXIT)
									if (strcmp(peek(stack_arg).lexeme, name_do.lexeme) == 0)
									{
										supkey = 0;
										wkey = 555;
										break;
									}	
								push(stack_arg, lexemka);
								if (id == ELSE)
									ikey = 0;
								if (ikey)
									Main_switch(stack_arg, tree_arg);
								else
									pop(stack_arg);
							}
						}
					}
					else
					{
						puts("not have ')'");
						return;
					}
				}
			}
			else
				if (peek(stack_arg).id == IDENT_NAME) //'IDENT_NAME'(переменная)
				{
					tl_unit lexemka = pop(stack_arg);
					int key = -1;

					if (peek(stack_arg).id == RP)
					{
						push(stack_arg, lexemka);
						key = get_var_value(&var_table, peek(stack_arg).lexeme);
					}
					else
					{
						push(stack_arg, lexemka);
						if (id = funcsravni(stack_arg))
						{
							if (id == -1)
								return;
							key = 1;
						}
						else
						{
							if (id == -1)
								return;
							key = 0;
						}
					}
					if (key)
					{
						if (key == -1)
						{
							puts("check var in if");
							return;
						}
						if (pop(stack_arg).id == RP) //'RP'(')')
						{
							while (1)// 'гоним до END If если нет then'
							{
								id = pop(stack_arg).id;
								if (id == THEN)
								{
									break;
								}
								if ((id == END) && (peek(stack_arg).id == CTRL_FLOW)) // 'END IF'
								{
									pop(stack_arg).id;
									break;
								}
								if (id == EOI)
								{
									puts("not have END IF");
									return;
								}
							}
							if (id == THEN)// 'THEN'
							{
								tl_unit lexemka;
								while (1)//'THEN при переменная 1'
								{
									lexemka = peek(stack_arg);
									if (((id = pop(stack_arg).id) == END) && (peek(stack_arg).id == CTRL_FLOW)) // 'END IF'
									{
										pop(stack_arg).id;
										break;
									}
									if (id == EOI)
									{
										puts("not have END IF");
										return;
									}
									if (id == EXIT)
										if (strcmp(peek(stack_arg).lexeme, name_do.lexeme) == 0)
										{
											supkey = 0;
											wkey = 555;
											break;
										}

									push(stack_arg, lexemka);
									if (id == ELSE)
										ikey = 0;
									if (ikey)
										Main_switch(stack_arg, tree_arg);
									else
										pop(stack_arg);
								}
							}
						}
						else
						{
							puts("not have ')'");
							return;
						}
					}
					else
					{
						if (id == -1)
							return;
						if (pop(stack_arg).id == RP) //'RP'	')'
						{
							while ((id = pop(stack_arg).id) != ELSE)// 'ELSE'
							{
								if ((oldid == END) && (id == CTRL_FLOW)) // 'END IF'
								{
									break;
								}
								if (id == EOI)
								{
									puts("not have END IF");
									return;
								}
								oldid = id;
							}
							if (id == ELSE)// 'ELSE'
							{
								tl_unit lexemka;
								while (1)// 'END'
								{
									lexemka = peek(stack_arg);
									if (((id = pop(stack_arg).id) == END) && (peek(stack_arg).id == CTRL_FLOW)) // 'END IF'
									{
										pop(stack_arg).id;
										break;
									}
									if (id == EOI)
									{
										puts("not have END IF");
										return;
									}
									if (id == EXIT)
										if (strcmp(peek(stack_arg).lexeme, name_do.lexeme) == 0)
										{
											supkey = 0;
											key = 0;
											wkey = 555;
											break;
										}
									push(stack_arg, lexemka);
									Main_switch(stack_arg, tree_arg);
								}
							}
						}
						else
						{
							puts("not have ')'");
							return;
						}
					}
				}
				else
				{
					puts("not have '(???)''");
					return;
				}
		}
		else
		{
			puts("not have '('");
			return 1;
		}
	}
	break;
	case IDENT_NAME:
		move_to_root(tree_arg);
		
		tl_unit name_var = current_tl;

		lookahead = pop(stack_arg);
		switch (lookahead.id)
		{
		case CLN: // loop name "myLoop : do while()"
			if (pop(stack_arg).id == DO)
			{
				switch (pop(stack_arg).id)
				{
				case WHILE_LOOP:
				{
					name_do = name_var;
					index_while = get_index(stack_arg) + 5;
					wkey = 0;
					int id = 0;
					if (pop(stack_arg).id == LP)//'LP'	'('
					{
						if ((id = peek(stack_arg).id) == INT_NUM)//'INT_NUM'(1/0)
						{
							if (strcmp(pop(stack_arg).lexeme, "0") == 0)
							{
								supkey = 0;
								id = rab_while(stack_arg, tree_arg, name_do);
								if (id == -1)
									return;
							}
							else
							{
								supkey = 1;
								id = rab_while(stack_arg, tree_arg, name_do);
								if (id == -1)
									return;
							}
						}
						else
							if (peek(stack_arg).id == IDENT_NAME) //'IDENT_NAME'(переменная)
							{
								if (id = funcsravni(stack_arg))
								{
									if (id == -1)
										return;

									supkey = 1;
									id = rab_while(stack_arg, tree_arg, name_do);
									if (id == -1)
										return;
								}
								else
								{
									if (id == -1)
										return;

									supkey = 0;
									id = rab_while(stack_arg, tree_arg, name_do);
									if (id == -1)
										return;
								}
							}
							else
							{
								puts("not have '(???)''");
								return;
							}
					}
					else
					{
						puts("not have '('");
						return 1;
					}
				}
				break;
				case IDENT_NAME:
					// FOR LOOP
					break;
				default:
					// error
					break;
				}
			}
			else
			{
				missing_token_err("DO");
			}
			break;
		case ASG_OP: // variable "x = 5"
			if (search_var(&var_table, current_tl.lexeme) < 0) // Error case
			{
				var_not_declared(current_tl.lexeme);
				break;
			}
			add_node(tree_arg, current_tl);

			add_node(tree_arg, lookahead);

			valsign_stack_init(&vs_stack);
			lookahead = peek(stack_arg);
			switch (lookahead.id)
			{
			case IDENT_NAME:
			{
				if (search_var(&var_table, lookahead.lexeme) < 0) // Error case
				{
					var_not_declared(lookahead.lexeme);
					break;
				}

				tl_unit next_var = lookahead;

				lookahead = pop(stack_arg);
				lookahead = peek(stack_arg);

				// Expression like "x = x + y + z * c"
				if (lookahead.id == ADD || lookahead.id == SUB || lookahead.id == MULT || lookahead.id == DIV)
				{
					int result = 0;
					int md_result = 0;

					push(stack_arg, next_var);
					char* value = (char*)malloc(LEXEME_LEN);
					_itoa(expr(stack_arg, tree_arg, &vs_stack, &result, &md_result, &var_table), value, 10);

					add_var_value(&var_table, current_tl.lexeme, value);

					free(value);

					tl_unit empty_node = { EMPTY, "" };
					add_node(tree_arg, empty_node);
					//...
				}
				else if (lookahead.id == LOGIC_EQ || lookahead.id == LOGIC_NEQ || lookahead.id == LOGIC_G
					|| lookahead.id == LOGIC_L || lookahead.id == LOGIC_GORE || lookahead.id == LOGIC_LORE)
				{
					add_node(tree_arg, next_var);

					push(stack_arg, next_var);
					char* value = (char*)malloc(LEXEME_LEN);
					_itoa(funcsravni(stack_arg), value, 10);

					add_var_value(&var_table, current_tl.lexeme, value);

					free(value);

					tl_unit empty_node = { EMPTY, "" };
					add_node(tree_arg, empty_node);
				}
				else
				{
					add_node(tree_arg, next_var);

					add_var_value(&var_table, current_tl.lexeme, next_var.lexeme);

					tl_unit empty_node = { EMPTY, "" };
					add_node(tree_arg, empty_node);
				}
			}
				break;
			case INT_NUM:
			{
				tl_unit next_num = lookahead;

				lookahead = pop(stack_arg);
				lookahead = peek(stack_arg);

				// Expression like "x = x + y + z * c"
				if (lookahead.id == ADD || lookahead.id == SUB || lookahead.id == MULT || lookahead.id == DIV)
				{
					int result = 0;
					int md_result = 0;

					push(stack_arg, next_num);
					char* value = (char*)malloc(LEXEME_LEN);
					_itoa(expr(stack_arg, tree_arg, &vs_stack, &result, &md_result, &var_table), value, 10);

					add_var_value(&var_table, current_tl.lexeme, value);

					free(value);

					tl_unit empty_node = { EMPTY, "" };
					add_node(tree_arg, empty_node);
					//...
				}
				else if (lookahead.id == LOGIC_EQ || lookahead.id == LOGIC_NEQ || lookahead.id == LOGIC_G
					|| lookahead.id == LOGIC_L || lookahead.id == LOGIC_GORE || lookahead.id == LOGIC_LORE)
				{
					add_node(tree_arg, next_num);

					push(stack_arg, next_num);
					char* value = (char*)malloc(LEXEME_LEN);
					_itoa(funcsravni(stack_arg), value, 10);

					add_var_value(&var_table, current_tl.lexeme, value);

					free(value);

					tl_unit empty_node = { EMPTY, "" };
					add_node(tree_arg, empty_node);
				}
				else
				{
					add_node(tree_arg, next_num);

					add_var_value(&var_table, current_tl.lexeme, next_num.lexeme);

					tl_unit empty_node = { EMPTY, "" };
					add_node(tree_arg, empty_node);
				}

			}
				break;
			default:
				break;
			}
			break;
		default:
			missing_token_err("CLN or ASG_OP");
			push(stack_arg, lookahead);
			break;
		}
		break;
	case DT_INT:
		move_to_root(tree_arg);
		add_node(tree_arg, current_tl);
		
		current_tl = pop(stack_arg);
		if (current_tl.id == DT_ASG_OP)
		{
			current_tl = pop(stack_arg);
			if (current_tl.id == IDENT_NAME)
			{
				add_var_name(&var_table, current_tl.lexeme); // Writing first variable to the table

				add_node(tree_arg, current_tl);

				current_tl = pop(stack_arg);
				if (current_tl.id == CM)
				{
					tl_unit last;
					while (current_tl.id != EOL && current_tl.id != EOI
								&& (current_tl.id == IDENT_NAME || current_tl.id == CM))
					{
						last = current_tl;
						current_tl = pop(stack_arg);

						if (current_tl.id == IDENT_NAME)
						{
							move_up(tree_arg);
							add_node(tree_arg, current_tl);

							add_var_name(&var_table, current_tl.lexeme);
						}
					}
					push(stack_arg, current_tl);

					if (last.id == CM)
					{
						missing_token_err("IDENT_NAME");
					}
				}
				else 
				{
					break;
				}
			}
			else
			{
				missing_token_err("IDNET_NAME");
			}
		}
		else
		{
			missing_token_err("DT_ASG_OP");
		}
		tl_unit empty_node = {EMPTY, ""};
		move_up(tree_arg);
		add_node(tree_arg, empty_node);

		break;
	case DT_DOUBLE:
		//...
		break;
	
	case EOL:
		break;
	case EOI:
		printf("EOI!\n");
		break;
	default:
		// errors
		break;
	}
}

/*
	tl_tree utility functions
*/
void add_node(tl_tree* tree, tl_unit tl_arg)
{
	if (tl_arg.id == EOL)
	{
		return;
	}

	tl_tree_unit* temp = (tl_tree_unit*)malloc(sizeof(tl_tree_unit));
	
	// Fields
	temp->tl = tl_arg;

	// Root
	if (tree->root == NULL)
	{
		tree->root = temp;
	}

	// Current change
	// Prev assignment
	temp->prev = tree->current;
	tree->current = temp;

	tree->current->index = 0;
	/*
	int i = 0;
	while (i < LINK_COUNT)
	{
		tree->current->links[i] = malloc(sizeof(struct _tl_tree_unit));
		tree->current->links[i]->tl.id = TREE_END;

		i++;
	}*/

	// Links
	if (tree->current->prev != NULL)
	{
		tree->current->prev->links[tree->current->prev->index] = temp;
		tree->current->prev->index++;
	}
}

void move_up(tl_tree* tree)
{
	tree->current = tree->current->prev;
}

void move_down(tl_tree* tree, int branch_num)
{
	tree->current = tree->current->links[branch_num];
}

void move_to_root(tl_tree* tree)
{
	while(tree->current != tree->root)
	{
		move_up(tree);
	}
}

/*
	parsing utility functions
*/

int rab_while(tl_stack* stack_arg, tl_tree* tree, tl_unit name_do)
{
	tl_unit lexemka;

	if (pop(stack_arg).id == RP) //'RP'		')'
	{
		while (1)
		{
			if (peek(stack_arg).id == EOI)
			{
				puts("gde END DO?");
				return -1;
			}
			if (peek(stack_arg).id == END)
			{
				lexemka = pop(stack_arg);
				if (peek(stack_arg).id == DO)
				{
					if (supkey == 1)
						stack_arg->index = index_while;
					pop(stack_arg);
					break;
				}
				push(stack_arg, lexemka);
			}
			if (supkey && wkey != 555)
			{
				if (peek(stack_arg).id == EXIT)
				{
					pop(stack_arg);
					if (strcmp(pop(stack_arg).lexeme, name_do.lexeme) == 0)
					{
						wkey = 555;
						supkey = 0;
					}
					else
					{
						puts("name do neveren!");
						return -1;
					}
				}
				Main_switch(stack_arg, tree);
			}
			else
				pop(stack_arg);
		}
	}
	else
	{
		puts("not have ')'");
		return 1;
	}
}

int funcsravni(tl_stack* stack_arg)
{
	int key = -1;
	char* str = (char*)malloc(30 * sizeof(char));

	str = peek(stack_arg).lexeme;
	int znach;
	if (peek(stack_arg).id == IDENT_NAME)
		znach = get_var_value(&var_table, pop(stack_arg).lexeme);
	else if (peek(stack_arg).id == INT_NUM)
		znach = atoi(pop(stack_arg).lexeme);
	else
	{
		puts("error: in sravni on first place no var/const");
		return -1;
	}

	int id = peek(stack_arg).id;

	if (id == LOGIC_EQ)//(==) 
	{
		char* str_next = (char*)malloc(30 * sizeof(char));
		str_next = peek(stack_arg).lexeme;
		int oldid = pop(stack_arg).id;
		int id = peek(stack_arg).id;

		if (id == IDENT_NAME)
		{
			int chislo = get_var_value(&var_table, pop(stack_arg).lexeme);

			if (znach == chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else if (id == INT_NUM)
		{
			int chislo = atoi(pop(stack_arg).lexeme);
			if (znach == chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else
		{
			printf("neschem sravnit': %s %s ...\n\n", str, str_next);
			return -1;
		}
	}
	if (id == LOGIC_L) //(<) 
	{
		char* str_next = (char*)malloc(30 * sizeof(char));
		str_next = peek(stack_arg).lexeme;
		int oldid = pop(stack_arg).id;
		int id = peek(stack_arg).id;

		if (id == IDENT_NAME)
		{
			int chislo = get_var_value(&var_table, pop(stack_arg).lexeme);

			if (znach < chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else if (id == INT_NUM)
		{
			int chislo = atoi(pop(stack_arg).lexeme);
			if (znach < chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else
		{
			printf("neschem sravnit': %s %s ...\n\n", str, str_next);
			return -1;
		}
	}
	if (id == LOGIC_G) //(>) 
	{
		char* str_next = (char*)malloc(30 * sizeof(char));
		str_next = peek(stack_arg).lexeme;
		int oldid = pop(stack_arg).id;
		int id = peek(stack_arg).id;

		if (id == IDENT_NAME)
		{
			int chislo = get_var_value(&var_table, pop(stack_arg).lexeme);

			if (znach > chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else if (id == INT_NUM)
		{
			int chislo = atoi(pop(stack_arg).lexeme);
			if (znach > chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else
		{
			printf("neschem sravnit': %s %s ...\n\n", str, str_next);
			return -1;
		}
	}
	if (id == LOGIC_GORE) //(>=) 
	{
		char* str_next = (char*)malloc(30 * sizeof(char));
		str_next = peek(stack_arg).lexeme;
		int oldid = pop(stack_arg).id;
		int id = peek(stack_arg).id;

		if (id == IDENT_NAME)
		{
			int chislo = get_var_value(&var_table, pop(stack_arg).lexeme);

			if (znach >= chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else if (id == INT_NUM)
		{
			int chislo = atoi(pop(stack_arg).lexeme);
			if (znach >= chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else
		{
			printf("neschem sravnit': %s %s ...\n\n", str, str_next);
			return -1;
		}
	}
	if (id == LOGIC_LORE) //(<=) 
	{
		char* str_next = (char*)malloc(30 * sizeof(char));
		str_next = peek(stack_arg).lexeme;
		int oldid = pop(stack_arg).id;
		int id = peek(stack_arg).id;

		if (id == IDENT_NAME)
		{
			int chislo = get_var_value(&var_table, pop(stack_arg).lexeme);

			if (znach <= chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else if (id == INT_NUM)
		{
			int chislo = atoi(pop(stack_arg).lexeme);
			if (znach <= chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else
		{
			printf("neschem sravnit': %s %s ...\n\n", str, str_next);
			return -1;
		}
	}
	if (id == LOGIC_NEQ) //(!=) 
	{
		char* str_next = (char*)malloc(30 * sizeof(char));
		str_next = peek(stack_arg).lexeme;
		int oldid = pop(stack_arg).id;
		int id = peek(stack_arg).id;

		if (id == IDENT_NAME)
		{
			int chislo = get_var_value(&var_table, pop(stack_arg).lexeme);

			if (znach != chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else if (id == INT_NUM)
		{
			int chislo = atoi(pop(stack_arg).lexeme);
			if (znach != chislo)
				key = 1; //true
			else
				key = 0; //false 
		}
		else
		{
			printf("neschem sravnit': %s %s ...\n\n", str, str_next);
			return -1;
		}
	}
	if (key == -1)
	{
		puts("error: posle var net logick");
		return -1;
	}
	return key;
}

/*
	add_node(tree_arg, end_of_branch);
	move_to_root(tree_arg);
*/

int expr(tl_stack* stack_arg, tl_tree* tree_arg, valsign_stack* vs_stack_arg, int* result, int* md_result, variable_table* table_arg)
{
	tl_unit temp;
	enum TOKEN_IDs temp_sign;
	tl_unit lookahead;
	int t1, t2, temp_result = 0;

	temp = pop(stack_arg);

	switch (temp.id)
	{
	case INT_NUM:
		add_node(tree_arg, temp);
		push_val(vs_stack_arg, atoi(temp.lexeme));
		
		if (vs_stack_arg->last_sign == MULT)
		{
			if (vs_stack_arg->last_operated_sign != EMPTY && vs_stack_arg->last_operated_sign != PROGRAM)
			{
				// Sign update
				pop_sign(vs_stack_arg);
				vs_stack_arg->last_sign = vs_stack_arg->pre_last_sign;
				temp_sign = pop_sign(vs_stack_arg);
				vs_stack_arg->pre_last_sign = peek_sign(vs_stack_arg);
				push_sign(vs_stack_arg, temp_sign);
				vs_stack_arg->last_operated_sign = MULT;

				t1 = pop_val(vs_stack_arg); pop_val(vs_stack_arg);
				t2 = *md_result;

				*md_result = t1 * t2;
				push_val(vs_stack_arg, *md_result);

			}
			else
			{
				// Sign update
				pop_sign(vs_stack_arg);
				vs_stack_arg->last_sign = vs_stack_arg->pre_last_sign;
				temp_sign = pop_sign(vs_stack_arg);
				vs_stack_arg->pre_last_sign = peek_sign(vs_stack_arg);
				push_sign(vs_stack_arg, temp_sign);
				vs_stack_arg->last_operated_sign = MULT;


				t1 = pop_val(vs_stack_arg);
				t2 = pop_val(vs_stack_arg);

				*md_result = t1 * t2;
				push_val(vs_stack_arg, *md_result);
			}
		}
		else if (vs_stack_arg->last_sign == DIV)
		{
			if (vs_stack_arg->last_operated_sign != EMPTY && vs_stack_arg->last_operated_sign != PROGRAM)
			{
				// Sign update
				pop_sign(vs_stack_arg);
				vs_stack_arg->last_sign = vs_stack_arg->pre_last_sign;
				temp_sign = pop_sign(vs_stack_arg);
				vs_stack_arg->pre_last_sign = peek_sign(vs_stack_arg);
				push_sign(vs_stack_arg, temp_sign);
				vs_stack_arg->last_operated_sign = DIV;

				t1 = pop_val(vs_stack_arg); pop_val(vs_stack_arg);
				t2 = *md_result;

				*md_result = t2 / t1;
				push_val(vs_stack_arg, *md_result);
			}
			else
			{
				// Sign update
				pop_sign(vs_stack_arg);
				vs_stack_arg->last_sign = vs_stack_arg->pre_last_sign;
				temp_sign = pop_sign(vs_stack_arg);
				vs_stack_arg->pre_last_sign = peek_sign(vs_stack_arg);
				push_sign(vs_stack_arg, temp_sign);
				vs_stack_arg->last_operated_sign = DIV;

				t1 = pop_val(vs_stack_arg);
				t2 = pop_val(vs_stack_arg);

				*md_result = t2 / t1;
				push_val(vs_stack_arg, *md_result);
			}
		}
		
		lookahead = peek(stack_arg);
		if (lookahead.id == EOI || lookahead.id == EOL)
		{
			int val_stop = vs_stack_arg->index_val;
			int sign_stop = vs_stack_arg->index_sign;

			while (vs_stack_arg->index_back_val < val_stop && vs_stack_arg->index_back_sign < sign_stop)
			{
				//???
				if(vs_stack_arg->pre_last_sign == PROGRAM)
				{
					return *md_result;
				}

				switch (back_pop_sign(vs_stack_arg))
				{
				case ADD:
					t1 = back_pop_val(vs_stack_arg);
					t2 = back_pop_val(vs_stack_arg);

					*result = t1 + t2;

					back_push_val(vs_stack_arg, *result);
					break;
				case SUB:
					t1 = back_pop_val(vs_stack_arg);
					t2 = back_pop_val(vs_stack_arg);

					*result = t1 - t2;

					back_push_val(vs_stack_arg, *result);
					break;
				default:
					break;
				}
			}

			return *result;
		}

		if (lookahead.id == ADD || lookahead.id == SUB)
		{
			*result += *md_result;
			*md_result = 0;
			vs_stack_arg->last_operated_sign = EMPTY;

			lookahead = pop(stack_arg);

			// Adding a node and pushing a sign to the stack
			add_node(tree_arg, lookahead);
			push_sign(vs_stack_arg, lookahead.id);

			// Sign update
			vs_stack_arg->pre_last_sign = vs_stack_arg->last_sign;
			vs_stack_arg->last_sign = lookahead.id;

			*result = expr(stack_arg, tree_arg, vs_stack_arg, result, md_result, table_arg);
		}
		else if (lookahead.id == MULT || lookahead.id == DIV)
		{
			lookahead = pop(stack_arg);

			// Adding a node and pushing a sign to the stack
			add_node(tree_arg, lookahead);
			push_sign(vs_stack_arg, lookahead.id);

			// Sign update
			vs_stack_arg->pre_last_sign = vs_stack_arg->last_sign;
			vs_stack_arg->last_sign = lookahead.id;

			*result = expr(stack_arg, tree_arg, vs_stack_arg, result, md_result, table_arg);
		}

		break;
	case IDENT_NAME:
		add_node(tree_arg, temp);
		push_val(vs_stack_arg, get_var_value(table_arg, temp.lexeme));

		if (vs_stack_arg->last_sign == MULT)
		{
			if (vs_stack_arg->last_operated_sign != EMPTY && vs_stack_arg->last_operated_sign != PROGRAM) // FIX
			{
				// Sign update
				pop_sign(vs_stack_arg);
				vs_stack_arg->last_sign = vs_stack_arg->pre_last_sign;
				temp_sign = pop_sign(vs_stack_arg);
				vs_stack_arg->pre_last_sign = peek_sign(vs_stack_arg);
				push_sign(vs_stack_arg, temp_sign);
				vs_stack_arg->last_operated_sign = MULT;

				t1 = pop_val(vs_stack_arg); pop_val(vs_stack_arg);
				t2 = *md_result;

				*md_result = t1 * t2;
				push_val(vs_stack_arg, *md_result);

			}
			else
			{
				// Sign update
				pop_sign(vs_stack_arg);
				vs_stack_arg->last_sign = vs_stack_arg->pre_last_sign;
				temp_sign = pop_sign(vs_stack_arg);
				vs_stack_arg->pre_last_sign = peek_sign(vs_stack_arg);
				push_sign(vs_stack_arg, temp_sign);
				vs_stack_arg->last_operated_sign = MULT;


				t1 = pop_val(vs_stack_arg);
				t2 = pop_val(vs_stack_arg);

				*md_result = t1 * t2;
				push_val(vs_stack_arg, *md_result);
			}
		}
		else if (vs_stack_arg->last_sign == DIV)
		{
			if (vs_stack_arg->last_operated_sign != EMPTY && vs_stack_arg->last_operated_sign != PROGRAM)
			{
				// Sign update
				pop_sign(vs_stack_arg);
				vs_stack_arg->last_sign = vs_stack_arg->pre_last_sign;
				temp_sign = pop_sign(vs_stack_arg);
				vs_stack_arg->pre_last_sign = peek_sign(vs_stack_arg);
				push_sign(vs_stack_arg, temp_sign);
				vs_stack_arg->last_operated_sign = DIV;

				t1 = pop_val(vs_stack_arg); pop_val(vs_stack_arg);
				t2 = *md_result;

				*md_result = t2 / t1;
				push_val(vs_stack_arg, *md_result);
			}
			else
			{
				// Sign update
				pop_sign(vs_stack_arg);
				vs_stack_arg->last_sign = vs_stack_arg->pre_last_sign;
				temp_sign = pop_sign(vs_stack_arg);
				vs_stack_arg->pre_last_sign = peek_sign(vs_stack_arg);
				push_sign(vs_stack_arg, temp_sign);
				vs_stack_arg->last_operated_sign = DIV;

				t1 = pop_val(vs_stack_arg);
				t2 = pop_val(vs_stack_arg);

				*md_result = t2 / t1;
				push_val(vs_stack_arg, *md_result);
			}
		}

		lookahead = peek(stack_arg);
		if (lookahead.id == EOI || lookahead.id == EOL)
		{
			int val_stop = vs_stack_arg->index_val;
			int sign_stop = vs_stack_arg->index_sign;

			while (vs_stack_arg->index_back_val < val_stop && vs_stack_arg->index_back_sign < sign_stop)
			{
				//???
				if (vs_stack_arg->pre_last_sign == PROGRAM)
				{
					return *md_result;
				}

				switch (back_pop_sign(vs_stack_arg))
				{
				case ADD:
					t1 = back_pop_val(vs_stack_arg);
					t2 = back_pop_val(vs_stack_arg);

					*result = t1 + t2;

					back_push_val(vs_stack_arg, *result);
					break;
				case SUB:
					t1 = back_pop_val(vs_stack_arg);
					t2 = back_pop_val(vs_stack_arg);

					*result = t1 - t2;

					back_push_val(vs_stack_arg, *result);
					break;
				default:
					break;
				}
			}

			return *result;
		}

		if (lookahead.id == ADD || lookahead.id == SUB)
		{
			*result += *md_result;
			*md_result = 0;
			vs_stack_arg->last_operated_sign = EMPTY;

			lookahead = pop(stack_arg);

			// Adding a node and pushing a sign to the stack
			add_node(tree_arg, lookahead);
			push_sign(vs_stack_arg, lookahead.id);

			// Sign update
			vs_stack_arg->pre_last_sign = vs_stack_arg->last_sign;
			vs_stack_arg->last_sign = lookahead.id;

			*result = expr(stack_arg, tree_arg, vs_stack_arg, result, md_result, table_arg);
		}
		else if (lookahead.id == MULT || lookahead.id == DIV)
		{
			lookahead = pop(stack_arg);

			// Adding a node and pushing a sign to the stack
			add_node(tree_arg, lookahead);
			push_sign(vs_stack_arg, lookahead.id);

			// Sign update
			vs_stack_arg->pre_last_sign = vs_stack_arg->last_sign;
			vs_stack_arg->last_sign = lookahead.id;

			*result = expr(stack_arg, tree_arg, vs_stack_arg, result, md_result, table_arg);
		}

		break;
	default:
		// error
		break;
	}
	
	//...
}

/*
	variable_table utility functions
*/

// Initialization
void table_init(variable_table* table_arg)
{
	table_arg->index = 0;

	int i = 0;
	while (i < TABLE_SIZE)
	{
		strcpy(table_arg->var_table[i].var_name, "EMPTY");
		table_arg->var_table[i].var_value = 0;

		i++;
	}
}

// Adds variable name(identifier) to a table, when meets it
int add_var_name(variable_table* table_arg, char* var_name)
{
	
	if (search_var(table_arg, var_name) < 0)
	{
		strcpy(table_arg->var_table[table_arg->index].var_name, var_name);
		table_arg->index++;
	}
	else
	{
		printf("Variable already exists!\n");
		return -1;
	}
}

// Adds value to an existing variable
void add_var_value(variable_table* table_arg, char* name, char* value) 
{
	int i, i2;

	if (isalpha(value[0])) // Case value is a variable
	{
		i = search_var(table_arg, name);
		i2 = search_var(table_arg, value);

		if (i >= 0 && i2 >= 0)
		{
			table_arg->var_table[i].var_value = table_arg->var_table[i2].var_value;
		}
		else
		{
			printf("First or second variable was not found!\n");
			return;
		}
	}
	else // Case value is literal
	{
		i = search_var(table_arg, name);
		if (i >= 0)
		{
			table_arg->var_table[i].var_value = atoi(value);
		}
		else
		{
			printf("Variable was not found!\n");
			return;
		}
	}
}

// Gets variables value
int get_var_value(variable_table* table_arg, char* var_name)
{
	int i = search_var(table_arg, var_name);
	if (i >= 0)
	{
		return table_arg->var_table[i].var_value;
	}
	else
	{
		printf("Variable was not found!\n");
		return;
	}
}

// Searches for a variable that already exists in the table by its name
int search_var(variable_table* table_arg, char* name)
{
	int i = 0;
	
	while (i < TABLE_SIZE) 
	{
		if (strcmp(table_arg->var_table[i].var_name, name) == 0) // strcmp() returns "0" on equal
		{
			return i;
		}

		if (strcmp(table_arg->var_table[i].var_name, "EMPTY") == 0) // If empty strings reached (means variable was not found)
		{
			return -1;
		}

		i++;
	}	

	printf("Variable was not found!\n");
	return -1;
}

/*
	val_stack utility functions
*/

void valsign_stack_init(valsign_stack* val_stack_arg)
{
	val_stack_arg->index_val = 1;
	val_stack_arg->index_back_val = 0;
	val_stack_arg->index_sign = 1;
	val_stack_arg->index_back_sign = 0;
	val_stack_arg->last_sign = EMPTY;
	val_stack_arg->pre_last_sign = EMPTY;
}

void valsign_stack_clear(valsign_stack* val_stack_arg)
{
	valsign_stack_init(val_stack_arg);

	int i = 0;

	while (i < VAL_STACK_SIZE)
	{
		val_stack_arg->signs[i] = EMPTY;
		val_stack_arg->values[i] = 0;

		i++;
	}
}

void push_val(valsign_stack* val_stack_arg, int value_arg)
{
	val_stack_arg->values[val_stack_arg->index_val] = value_arg;
	val_stack_arg->index_val++;
}

int pop_val(valsign_stack* val_stack_arg)
{
	if (val_stack_arg->index_val - 1 < 0)
	{
		return val_stack_arg->values[val_stack_arg->index_val];
	}
	else
	{
		return val_stack_arg->values[--val_stack_arg->index_val];
	}
}

int back_pop_val(valsign_stack* val_stack_arg)
{
	if (val_stack_arg->index_back_val + 1 > VAL_STACK_SIZE)
	{
		return val_stack_arg->values[val_stack_arg->index_back_val];
	}
	else
	{
		//val_stack_arg->index_val--;
		return val_stack_arg->values[++val_stack_arg->index_back_val];
	}
}

void back_push_val(valsign_stack* val_stack_arg, int value_arg)
{
	val_stack_arg->values[val_stack_arg->index_back_val] = value_arg;
	val_stack_arg->index_back_val--;
}

int set_back_val_index(valsign_stack* val_stack_arg)
{
	int i = 0;
	while (val_stack_arg->signs[i] != SUB)
	{
		i++;
	}

	val_stack_arg->index_back_val = i - 1;
	return i - 1;
}

int peek_val(valsign_stack* val_stack_arg)
{
	if (val_stack_arg->index_val == 0)
	{
		return val_stack_arg->values[val_stack_arg->index_val];
	}
	else
	{
		return val_stack_arg->values[val_stack_arg->index_val - 1];
	}
}

void push_sign(valsign_stack* val_stack_arg, enum TOKEN_IDs sign_arg)
{
	val_stack_arg->signs[val_stack_arg->index_sign] = sign_arg;
	val_stack_arg->index_sign++;
}

void back_push_sign(valsign_stack* val_stack_arg, enum TOJEN_IDs sign_arg)
{
	val_stack_arg->signs[val_stack_arg->index_back_sign] = sign_arg;
	val_stack_arg->index_back_sign--;
}

enum TOKEN_IDs pop_sign(valsign_stack* val_stack_arg)
{
	if (val_stack_arg->index_sign - 1 < 0)
	{
		return val_stack_arg->signs[val_stack_arg->index_sign];
	}
	else
	{
		return val_stack_arg->signs[--val_stack_arg->index_sign];
	}
}

enum TOKEN_IDs back_pop_sign(valsign_stack* val_stack_arg)
{
	if (val_stack_arg->index_back_sign + 1 > VAL_STACK_SIZE)
	{
		return val_stack_arg->signs[val_stack_arg->index_back_sign];
	}
	else
	{
		//val_stack_arg->index_val--;
		return val_stack_arg->signs[++val_stack_arg->index_back_sign];
	}
}

enum TOKEN_IDs peek_sign(valsign_stack* val_stack_arg)
{
	if (val_stack_arg->index_sign == 0)
	{
		return val_stack_arg->signs[val_stack_arg->index_sign];
	}
	else
	{
		return val_stack_arg->signs[val_stack_arg->index_sign - 1];
	}
}