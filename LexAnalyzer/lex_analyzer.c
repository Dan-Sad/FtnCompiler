#pragma once
#include "lex_analyzer.h"

extern tl_stack toklex_stack;

/*
	lexical_analyzer utility functions
*/

// Starts the lexical analyzer and
// fills the stack with tokenized data
void start_lex(FILE* input, tl_stack* stack_arg)
{
	char current_symbol, next_symbol;

	char* tmp_lexeme_string = (char*)malloc(LEXEME_LEN);
	if (tmp_lexeme_string == NULL)
	{
		printf("Could not allocate memory for temporary string!\n");
		return;
	}

	tl_unit current_tl;

	current_tl.id = EOI;
	tmp_lexeme_string[0] = '\0';
	strcpy(current_tl.lexeme, tmp_lexeme_string);
	push(&toklex_stack, current_tl);

	while (!feof(input))
	{
		current_symbol = fgetc(input);

		switch (current_symbol)
		{
		//case SPACE: 
			//continue;
			//break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			current_tl.id = readt_number(current_symbol, input, tmp_lexeme_string);

			strcpy(current_tl.lexeme, tmp_lexeme_string);
			
			push(&toklex_stack, current_tl);
			break;
		case '-':
			// If next is num -> readt_number()
			// Else -> Math operator "x-y"
			next_symbol = fgetc(input);
			if (isdigit(next_symbol))
			{
				tmp_lexeme_string[0] = current_symbol;

				int str_index = 1;
				current_tl.id = readt_number_i(next_symbol, input, tmp_lexeme_string, str_index);

				strcpy(current_tl.lexeme, tmp_lexeme_string);

				push(&toklex_stack, current_tl);
			}
			else
			{
				current_tl.id = SUB;
				
				readt(current_symbol, input, tmp_lexeme_string, 1);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			break;
		case '+':
			current_tl.id = ADD;

			readt(current_symbol, input, tmp_lexeme_string, 1);
			strcpy(current_tl.lexeme, tmp_lexeme_string);
			push(&toklex_stack, current_tl);
			break;
		case '*':
			current_tl.id = MULT;

			readt(current_symbol, input, tmp_lexeme_string, 1);
			strcpy(current_tl.lexeme, tmp_lexeme_string);
			push(&toklex_stack, current_tl);
			break;
		case '/':
			current_tl.id = DIV;

			readt(current_symbol, input, tmp_lexeme_string, 1);
			strcpy(current_tl.lexeme, tmp_lexeme_string);
			push(&toklex_stack, current_tl);
			break;
		case '>':
			next_symbol = fgetc(input);
			if (next_symbol == '=')
			{
				current_tl.id = LOGIC_GORE;

				readt(current_symbol, input, tmp_lexeme_string, 2);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			else
			{
				ungetc(next_symbol, input);

				current_tl.id = LOGIC_G;

				readt(current_symbol, input, tmp_lexeme_string, 1);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			break;
		case '<':
			next_symbol = fgetc(input);
			if (next_symbol == '=')
			{
				ungetc(next_symbol, input);

				current_tl.id = LOGIC_LORE;

				readt(current_symbol, input, tmp_lexeme_string, 2);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			else
			{
				ungetc(next_symbol, input);

				current_tl.id = LOGIC_L;

				readt(current_symbol, input, tmp_lexeme_string, 1);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			break;
		case '=':
			next_symbol = fgetc(input);
			if (next_symbol == '=')
			{
				ungetc(next_symbol, input);

				current_tl.id = LOGIC_EQ;

				readt(current_symbol, input, tmp_lexeme_string, 2);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			else
			{
				ungetc(next_symbol, input);

				current_tl.id = ASG_OP;

				readt(current_symbol, input, tmp_lexeme_string, 1);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			break;
		case '!':
			next_symbol = fgetc(input);
			if (next_symbol == '=')
			{
				ungetc(next_symbol, input);

				current_tl.id = LOGIC_NEQ;

				readt(current_symbol, input, tmp_lexeme_string, 2);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			else
			{
				unidentified_token_err();
			}
			break;
		case '(':
			current_tl.id = LP;

			readt(current_symbol, input, tmp_lexeme_string, 1);
			strcpy(current_tl.lexeme, tmp_lexeme_string);
			push(&toklex_stack, current_tl);

			break;
		case ')':
			current_tl.id = RP;

			readt(current_symbol, input, tmp_lexeme_string, 1);
			strcpy(current_tl.lexeme, tmp_lexeme_string);
			push(&toklex_stack, current_tl);

			break;
		case SPACE:
			break;
		case TAB:
			break;
		case EOF:
			break;
		case '\n':
			current_tl.id = EOL;

			readt(current_symbol, input, tmp_lexeme_string, 1);
			strcpy(current_tl.lexeme, tmp_lexeme_string);
			push(&toklex_stack, current_tl);

			break;
		case ':':
			next_symbol = fgetc(input);

			if (next_symbol == ':')
			{
				ungetc(next_symbol, input);

				current_tl.id = DT_ASG_OP;

				readt(current_symbol, input, tmp_lexeme_string, 2);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			else
			{
				ungetc(next_symbol, input);

				current_tl.id = CLN;

				readt(current_symbol, input, tmp_lexeme_string, 1);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}

			break;
		case ',':
			current_tl.id = CM;

			readt(current_symbol, input, tmp_lexeme_string, 1);
			strcpy(current_tl.lexeme, tmp_lexeme_string);
			push(&toklex_stack, current_tl);
			break;
		case 'd':
			next_symbol = fgetc(input);
			if (next_symbol == 'o')
			{
				char tmp_lookahead = fgetc(input);
				if (tmp_lookahead == 'u')
				{
					tmp_lexeme_string[0] = current_symbol;

					current_symbol = next_symbol;
					next_symbol = tmp_lookahead;
					tmp_lookahead = fgetc(input);

					if (tmp_lookahead == 'b')
					{
						tmp_lexeme_string[1] = current_symbol;

						current_symbol = next_symbol;
						next_symbol = tmp_lookahead;
						tmp_lookahead = fgetc(input);

						if (tmp_lookahead == 'l')
						{
							tmp_lexeme_string[2] = current_symbol;

							current_symbol = next_symbol;
							next_symbol = tmp_lookahead;
							tmp_lookahead = fgetc(input);

							if (tmp_lookahead == 'e')
							{
								tmp_lexeme_string[3] = current_symbol;

								current_symbol = next_symbol;
								next_symbol = tmp_lookahead;
								tmp_lookahead = fgetc(input);

								if (isalnum(tmp_lookahead) || tmp_lookahead == '_')
								{
									current_tl.id = IDENT_NAME;

									tmp_lexeme_string[4] = current_symbol;
									tmp_lexeme_string[5] = next_symbol;

									readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 6);
									strcpy(current_tl.lexeme, tmp_lexeme_string);
									push(&toklex_stack, current_tl);
								}
								else
								{
									ungetc(tmp_lookahead, input); // Returning lookahead symbol back to the file

									current_tl.id = DT_DOUBLE;

									tmp_lexeme_string[4] = current_symbol;
									tmp_lexeme_string[5] = next_symbol;
									tmp_lexeme_string[6] = '\0';

									strcpy(current_tl.lexeme, tmp_lexeme_string);
									push(&toklex_stack, current_tl);
								}
							}
							else
							{
								current_tl.id = IDENT_NAME;

								tmp_lexeme_string[3] = current_symbol;
								tmp_lexeme_string[4] = next_symbol;

								readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 5);
								strcpy(current_tl.lexeme, tmp_lexeme_string);
								push(&toklex_stack, current_tl);
							}
						}
						else
						{
							current_tl.id = IDENT_NAME;

							tmp_lexeme_string[2] = current_symbol;
							tmp_lexeme_string[3] = next_symbol;

							readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 4);
							strcpy(current_tl.lexeme, tmp_lexeme_string);
							push(&toklex_stack, current_tl);
						}
					}
					else
					{
						current_tl.id = IDENT_NAME;

						tmp_lexeme_string[1] = current_symbol;
						tmp_lexeme_string[2] = next_symbol;

						readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 3);
						strcpy(current_tl.lexeme, tmp_lexeme_string);
						push(&toklex_stack, current_tl);
					}
				}
				else if (isalnum(tmp_lookahead) || tmp_lookahead == '_')
				{
					current_tl.id = IDENT_NAME;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;

					readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 2);
					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
				else
				{
					ungetc(tmp_lookahead, input); // Returning lookahead symbol back to the file

					current_tl.id = DO;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;
					tmp_lexeme_string[2] = '\0';

					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
			}
			else
			{
				ungetc(next_symbol, input);

				current_tl.id = IDENT_NAME;

				readt_ident(current_symbol, input, tmp_lexeme_string);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			break;
		case 'e':
			next_symbol = fgetc(input);
			if (next_symbol == 'l')
			{
				char tmp_lookahead = fgetc(input);
				if (tmp_lookahead == 's')
				{
					tmp_lexeme_string[0] = current_symbol;

					current_symbol = next_symbol;
					next_symbol = tmp_lookahead;
					tmp_lookahead = fgetc(input);

					if (tmp_lookahead == 'e')
					{
						tmp_lexeme_string[1] = current_symbol;

						current_symbol = next_symbol;
						next_symbol = tmp_lookahead;
						tmp_lookahead = fgetc(input);

						if (isalnum(tmp_lookahead) || tmp_lookahead == '_')
						{
							current_tl.id = IDENT_NAME;

							tmp_lexeme_string[2] = current_symbol;
							tmp_lexeme_string[3] = next_symbol;

							readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 4);
							strcpy(current_tl.lexeme, tmp_lexeme_string);
							push(&toklex_stack, current_tl);
						}
						else
						{
							ungetc(tmp_lookahead, input); // Returning lookahead symbol back to the file

							current_tl.id = ELSE;

							tmp_lexeme_string[2] = current_symbol;
							tmp_lexeme_string[3] = next_symbol;
							tmp_lexeme_string[4] = '\0';

							strcpy(current_tl.lexeme, tmp_lexeme_string);
							push(&toklex_stack, current_tl);
						}
					}
					else
					{
						current_tl.id = IDENT_NAME;

						tmp_lexeme_string[1] = current_symbol;
						tmp_lexeme_string[2] = next_symbol;

						readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 3);
						strcpy(current_tl.lexeme, tmp_lexeme_string);
						push(&toklex_stack, current_tl);
					}
				}
				else
				{
					current_tl.id = IDENT_NAME;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;

					readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 2);
					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
			}
			else if (next_symbol == 'n')
			{
				char tmp_lookahead = fgetc(input);
				if (tmp_lookahead == 'd')
				{
					tmp_lexeme_string[0] = current_symbol;

					current_symbol = next_symbol;
					next_symbol = tmp_lookahead;
					tmp_lookahead = fgetc(input);

					if (isalnum(tmp_lookahead) || tmp_lookahead == '_')
					{
						current_tl.id = IDENT_NAME;

						tmp_lexeme_string[1] = current_symbol;
						tmp_lexeme_string[2] = next_symbol;

						readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 3);
						strcpy(current_tl.lexeme, tmp_lexeme_string);
						push(&toklex_stack, current_tl);
					}
					else
					{
						ungetc(tmp_lookahead, input); // Returning lookahead symbol back to the file

						current_tl.id = END;

						tmp_lexeme_string[1] = current_symbol;
						tmp_lexeme_string[2] = next_symbol;
						tmp_lexeme_string[3] = '\0';

						strcpy(current_tl.lexeme, tmp_lexeme_string);
						push(&toklex_stack, current_tl);
					}
				}
				else
				{
					current_tl.id = IDENT_NAME;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;

					readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 2);
					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
			}
			else if (next_symbol == 'x')
			{
				char tmp_lookahead = fgetc(input);
				if (tmp_lookahead == 'i')
				{
					tmp_lexeme_string[0] = current_symbol;

					current_symbol = next_symbol;
					next_symbol = tmp_lookahead;
					tmp_lookahead = fgetc(input);

					if (tmp_lookahead == 't')
					{
						tmp_lexeme_string[1] = current_symbol;

						current_symbol = next_symbol;
						next_symbol = tmp_lookahead;
						tmp_lookahead = fgetc(input);

						if (isalnum(tmp_lookahead) || tmp_lookahead == '_')
						{
							current_tl.id = IDENT_NAME;

							tmp_lexeme_string[2] = current_symbol;
							tmp_lexeme_string[3] = next_symbol;

							readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 4);
							strcpy(current_tl.lexeme, tmp_lexeme_string);
							push(&toklex_stack, current_tl);
						}
						else
						{
							ungetc(tmp_lookahead, input); // Returning lookahead symbol back to the file

							current_tl.id = EXIT;

							tmp_lexeme_string[2] = current_symbol;
							tmp_lexeme_string[3] = next_symbol;
							tmp_lexeme_string[4] = '\0';

							strcpy(current_tl.lexeme, tmp_lexeme_string);
							push(&toklex_stack, current_tl);
						}
					}
					else
					{
						current_tl.id = IDENT_NAME;

						tmp_lexeme_string[1] = current_symbol;
						tmp_lexeme_string[2] = next_symbol;

						readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 3);
						strcpy(current_tl.lexeme, tmp_lexeme_string);
						push(&toklex_stack, current_tl);
					}
				}
				else
				{
					current_tl.id = IDENT_NAME;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;

					readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 2);
					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
			}
			else
			{
				ungetc(next_symbol, input);

				current_tl.id = IDENT_NAME;

				readt_ident(current_symbol, input, tmp_lexeme_string);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}

			break;
		case 'i':
			next_symbol = fgetc(input);

			if (next_symbol == 'f') // "if" key-word case
			{
				char tmp_lookahead = fgetc(input);
				if (isalnum(tmp_lookahead) || tmp_lookahead == '_')
				{
					current_tl.id = IDENT_NAME;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;

					readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 2);
					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
				else
				{
					ungetc(tmp_lookahead, input); // Returning lookahead symbol back to the file

					current_tl.id = CTRL_FLOW;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;
					tmp_lexeme_string[2] = '\0';

					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
			}
			else if (next_symbol == 'n')
			{
				char tmp_lookahead = fgetc(input);
				if (tmp_lookahead == 't')
				{
					tmp_lexeme_string[0] = current_symbol;
					
					current_symbol = next_symbol;
					next_symbol = tmp_lookahead;
					tmp_lookahead = fgetc(input);

					if (isalnum(tmp_lookahead) || tmp_lookahead == '_')
					{
						current_tl.id = IDENT_NAME;

						tmp_lexeme_string[1] = current_symbol;
						tmp_lexeme_string[2] = next_symbol;

						readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 3);
						strcpy(current_tl.lexeme, tmp_lexeme_string);
						push(&toklex_stack, current_tl);
					}
					else
					{
						ungetc(tmp_lookahead, input); // Returning lookahead symbol back to the file

						current_tl.id = DT_INT; 

						tmp_lexeme_string[1] = current_symbol;
						tmp_lexeme_string[2] = next_symbol;
						tmp_lexeme_string[3] = '\0';

						strcpy(current_tl.lexeme, tmp_lexeme_string);
						push(&toklex_stack, current_tl);
					}
				}
				else
				{
					current_tl.id = IDENT_NAME;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;

					readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 2);
					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
			}
			else
			{
				ungetc(next_symbol, input);

				current_tl.id = IDENT_NAME;

				readt_ident(current_symbol, input, tmp_lexeme_string);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			
			break;
		case 'p':
			next_symbol = fgetc(input);
			if (next_symbol == 'r')
			{
				char tmp_lookahead = fgetc(input);
				if (tmp_lookahead == 'o')
				{
					tmp_lexeme_string[0] = current_symbol;

					current_symbol = next_symbol;
					next_symbol = tmp_lookahead;
					tmp_lookahead = fgetc(input);
					
					if (tmp_lookahead == 'g')
					{
						tmp_lexeme_string[1] = current_symbol;

						current_symbol = next_symbol;
						next_symbol = tmp_lookahead;
						tmp_lookahead = fgetc(input);

						if (tmp_lookahead == 'r')
						{
							tmp_lexeme_string[2] = current_symbol;

							current_symbol = next_symbol;
							next_symbol = tmp_lookahead;
							tmp_lookahead = fgetc(input);

							if (tmp_lookahead == 'a')
							{
								tmp_lexeme_string[3] = current_symbol;

								current_symbol = next_symbol;
								next_symbol = tmp_lookahead;
								tmp_lookahead = fgetc(input);

								if (tmp_lookahead == 'm')
								{
									tmp_lexeme_string[4] = current_symbol;

									current_symbol = next_symbol;
									next_symbol = tmp_lookahead;
									tmp_lookahead = fgetc(input);

									if (isalnum(tmp_lookahead) || tmp_lookahead == '_')
									{
										current_tl.id = IDENT_NAME;

										tmp_lexeme_string[5] = current_symbol;
										tmp_lexeme_string[6] = next_symbol;

										readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 7);
										strcpy(current_tl.lexeme, tmp_lexeme_string);
										push(&toklex_stack, current_tl);
									}
									else
									{
										ungetc(tmp_lookahead, input); // Returning lookahead symbol back to the file

										current_tl.id = PROGRAM;

										tmp_lexeme_string[5] = current_symbol;
										tmp_lexeme_string[6] = next_symbol;
										tmp_lexeme_string[7] = '\0';

										strcpy(current_tl.lexeme, tmp_lexeme_string);
										push(&toklex_stack, current_tl);
									}
								}
								else
								{
									current_tl.id = IDENT_NAME;

									tmp_lexeme_string[5] = current_symbol;
									tmp_lexeme_string[6] = next_symbol;

									readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 7);
									strcpy(current_tl.lexeme, tmp_lexeme_string);
									push(&toklex_stack, current_tl);
								}
							}
							else
							{
								current_tl.id = IDENT_NAME;

								tmp_lexeme_string[3] = current_symbol;
								tmp_lexeme_string[4] = next_symbol;

								readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 5);
								strcpy(current_tl.lexeme, tmp_lexeme_string);
								push(&toklex_stack, current_tl);
							}
						}
						else
						{
							current_tl.id = IDENT_NAME;

							tmp_lexeme_string[2] = current_symbol;
							tmp_lexeme_string[3] = next_symbol;

							readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 4);
							strcpy(current_tl.lexeme, tmp_lexeme_string);
							push(&toklex_stack, current_tl);
						}
					}
					else
					{
						current_tl.id = IDENT_NAME;

						tmp_lexeme_string[1] = current_symbol;
						tmp_lexeme_string[2] = next_symbol;

						readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 3);
						strcpy(current_tl.lexeme, tmp_lexeme_string);
						push(&toklex_stack, current_tl);
					}
				}
				else
				{
					current_tl.id = IDENT_NAME;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;

					readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 2);
					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
			}
			else
			{
				ungetc(next_symbol, input);

				current_tl.id = IDENT_NAME;

				readt_ident(current_symbol, input, tmp_lexeme_string);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}

			break;
		case 't':
			next_symbol = fgetc(input);
			if (next_symbol == 'h') // Key word case ("then")
			{
				char tmp_lookahead = fgetc(input);
				if (tmp_lookahead == 'e')
				{
					tmp_lexeme_string[0] = current_symbol;

					current_symbol = next_symbol;
					next_symbol = tmp_lookahead;
					tmp_lookahead = fgetc(input);

					if (tmp_lookahead == 'n')
					{
						tmp_lexeme_string[1] = current_symbol;

						current_symbol = next_symbol;
						next_symbol = tmp_lookahead;
						tmp_lookahead = fgetc(input);

						if (isalnum(tmp_lookahead) || tmp_lookahead == '_')
						{
							current_tl.id = IDENT_NAME;

							tmp_lexeme_string[2] = current_symbol;
							tmp_lexeme_string[3] = next_symbol;

							readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 4);
							strcpy(current_tl.lexeme, tmp_lexeme_string);
							push(&toklex_stack, current_tl);
						}
						else
						{
							ungetc(tmp_lookahead, input); // Returning lookahead symbol back to the file

							current_tl.id = THEN;

							tmp_lexeme_string[2] = current_symbol;
							tmp_lexeme_string[3] = next_symbol;
							tmp_lexeme_string[4] = '\0';

							strcpy(current_tl.lexeme, tmp_lexeme_string);
							push(&toklex_stack, current_tl);
						}
					}
					else
					{
						current_tl.id = IDENT_NAME;

						tmp_lexeme_string[1] = current_symbol;
						tmp_lexeme_string[2] = next_symbol;

						readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 3);
						strcpy(current_tl.lexeme, tmp_lexeme_string);
						push(&toklex_stack, current_tl);
					}
				}
				else
				{
					current_tl.id = IDENT_NAME;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;

					readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 2);
					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
			}
			else
			{
				ungetc(next_symbol, input);

				current_tl.id = IDENT_NAME;

				readt_ident(current_symbol, input, tmp_lexeme_string);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			break;
		case 'w':
			next_symbol = fgetc(input);
			if (next_symbol == 'h')
			{
				char tmp_lookahead = fgetc(input);
				if (tmp_lookahead == 'i')
				{
					tmp_lexeme_string[0] = current_symbol;

					current_symbol = next_symbol;
					next_symbol = tmp_lookahead;
					tmp_lookahead = fgetc(input);

					if (tmp_lookahead == 'l')
					{
						tmp_lexeme_string[1] = current_symbol;

						current_symbol = next_symbol;
						next_symbol = tmp_lookahead;
						tmp_lookahead = fgetc(input);

						if (tmp_lookahead == 'e')
						{
							tmp_lexeme_string[2] = current_symbol;

							current_symbol = next_symbol;
							next_symbol = tmp_lookahead;
							tmp_lookahead = fgetc(input);

							if (isalnum(tmp_lookahead) || tmp_lookahead == '_')
							{
								current_tl.id = IDENT_NAME;

								tmp_lexeme_string[3] = current_symbol;
								tmp_lexeme_string[4] = next_symbol;

								readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 5);
								strcpy(current_tl.lexeme, tmp_lexeme_string);
								push(&toklex_stack, current_tl);
							}
							else
							{
								ungetc(tmp_lookahead, input); // Returning lookahead symbol back to the file

								current_tl.id = WHILE_LOOP;

								tmp_lexeme_string[3] = current_symbol;
								tmp_lexeme_string[4] = next_symbol;
								tmp_lexeme_string[5] = '\0';

								strcpy(current_tl.lexeme, tmp_lexeme_string);
								push(&toklex_stack, current_tl);
							}
						}
						else
						{
							current_tl.id = IDENT_NAME;

							tmp_lexeme_string[2] = current_symbol;
							tmp_lexeme_string[3] = next_symbol;

							readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 4);
							strcpy(current_tl.lexeme, tmp_lexeme_string);
							push(&toklex_stack, current_tl);
						}
					}
					else
					{
						current_tl.id = IDENT_NAME;

						tmp_lexeme_string[1] = current_symbol;
						tmp_lexeme_string[2] = next_symbol;

						readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 3);
						strcpy(current_tl.lexeme, tmp_lexeme_string);
						push(&toklex_stack, current_tl);
					}
				}
				else
				{
					current_tl.id = IDENT_NAME;

					tmp_lexeme_string[0] = current_symbol;
					tmp_lexeme_string[1] = next_symbol;

					readt_ident_i(tmp_lookahead, input, tmp_lexeme_string, 2);
					strcpy(current_tl.lexeme, tmp_lexeme_string);
					push(&toklex_stack, current_tl);
				}
			}
			else
			{
				ungetc(next_symbol, input);

				current_tl.id = IDENT_NAME;

				readt_ident(current_symbol, input, tmp_lexeme_string);
				strcpy(current_tl.lexeme, tmp_lexeme_string);
				push(&toklex_stack, current_tl);
			}
			break;
			//case 'd': [do, double] Ready
			//case 'e': [end, else, exit] Ready
			//case 'i': [if, int] Ready
			//case 'p': [program] Ready
			//case 't': [then] Ready
			//case 'w': [while] Ready
		case 'a':
		case 'b':
		case 'c':
		case 'f':
		case 'g':
		case 'h':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'q':
		case 'r':
		case 's':
		case 'u':
		case 'v':
		case 'x':
		case 'y':
		case 'z':
			current_tl.id = IDENT_NAME;

			readt_ident(current_symbol, input, tmp_lexeme_string);
			strcpy(current_tl.lexeme, tmp_lexeme_string);
			push(&toklex_stack, current_tl);
			break;
		default:
			unidentified_token_err();
			break;
		}
	}

	free(tmp_lexeme_string);
}

// Reads numbers (int/double)
enum TOKEN_IDs readt_number(char first_ch, FILE* input, char* output_str)
{
	int str_index = 0;  // String position index
	enum TOKEN_IDs flag = INT_NUM; // Shows the type of number lexeme (int/double)
	char cur_ch = first_ch; // Current character 

	while (cur_ch >= '0' && cur_ch <= '9') // Writes whole "int" number OR integer part of "double" number
	{
		output_str[str_index] = cur_ch;
		cur_ch = fgetc(input);
		str_index++;
	}

	// If dot ['.'] -> add dot to the output string
	// and continue looking for numbers
	if (cur_ch == '.')
	{
		flag = DOUBLE_NUM;

		output_str[str_index] = cur_ch;
		cur_ch = fgetc(input);
		str_index++;

		while (cur_ch >= '0' && cur_ch <= '9') // Writes fractional part of the "double" number
		{
			output_str[str_index] = cur_ch;
			cur_ch = fgetc(input);
			str_index++;
		}
	}

	output_str[str_index] = '\0'; // NULL-Terminating string
	ungetc(cur_ch, input); // Putting the last read non-number character to the file stream
	return flag;
}

enum TOKEN_IDs readt_number_i(char first_ch, FILE* input, char* output_str, int predef_index)
{
	int str_index = predef_index;  // String position index
	enum TOKEN_IDs flag = INT_NUM; // Shows the type of number lexeme (int/double)
	char cur_ch = first_ch; // Current character 

	while (cur_ch >= '0' && cur_ch <= '9') // Writes whole "int" number OR integer part of "double" number
	{
		output_str[str_index] = cur_ch;
		cur_ch = fgetc(input);
		str_index++;
	}

	// If dot ['.'] -> add dot to the output string
	// and continue looking for numbers
	if (cur_ch == '.')
	{
		flag = DOUBLE_NUM;

		output_str[str_index] = cur_ch;
		cur_ch = fgetc(input);
		str_index++;

		while (cur_ch >= '0' && cur_ch <= '9') // Writes fractional part of the "double" number
		{
			output_str[str_index] = cur_ch;
			cur_ch = fgetc(input);
			str_index++;
		}
	}

	output_str[str_index] = '\0'; // NULL-Terminating string
	ungetc(cur_ch, input); // Putting the last read non-number character to the file stream
	return flag;
}

void readt_ident(char first_ch, FILE* input, char* output_str)
{
	int str_index = 0;  // String position index
	char cur_ch = first_ch; // Current character 

	while (isalnum(cur_ch) || cur_ch == '_')
	{
		output_str[str_index] = cur_ch;
		cur_ch = fgetc(input);
		str_index++;
	}

	output_str[str_index] = '\0';
	ungetc(cur_ch, input);
}

void readt_ident_i(char first_ch, FILE* input, char* output_str, int predef_index)
{
	int str_index = predef_index;  // String position index
	char cur_ch = first_ch; // Current character 

	while (isalnum(cur_ch) || cur_ch == '_')
	{
		output_str[str_index] = cur_ch;
		cur_ch = fgetc(input);
		str_index++;
	}

	output_str[str_index] = '\0';
	ungetc(cur_ch, input);
}

void readt(char first_ch, FILE* input, char* output_str, int symbols_count)
{
	int str_index = 0;  // String position index
	char cur_ch = first_ch; // Current character 

	while (str_index != symbols_count)
	{
		output_str[str_index] = cur_ch;
		cur_ch = fgetc(input);
		str_index++;
	}

	output_str[str_index] = '\0';
	ungetc(cur_ch, input);
}

/*
	tl_stack & tl_unit utility functions
*/

// Stack initialization
void stack_init(tl_stack* stack_arg)
{
	stack_arg->index = 0;
	
	int i = 0;
	while (i < STACK_SIZE)
	{
		unit_init(stack_arg->stack + i);
		i++;
	}
}

// Unit initialization
void unit_init(tl_unit* unit_arg)
{
	unit_arg->id = EMPTY;
}

// Pushes a tl_arg into stack_arg 
void push(tl_stack* stack_arg, tl_unit tl_arg)
{
	strcpy(stack_arg->stack[stack_arg->index].lexeme, tl_arg.lexeme);
	stack_arg->stack[stack_arg->index].id = tl_arg.id;
	stack_arg->index++;
}

// Pops a tl_unit from the stack
// If index == 0 - "peeks" a tl_unit
// from the stack without decrementing
// to prevent "out of range"
tl_unit pop(tl_stack* stack_arg)
{
	if (stack_arg->index - 1 < 0)
	{
		return stack_arg->stack[stack_arg->index];
	}
	else
	{
		return stack_arg->stack[--stack_arg->index];
	}
}

// Peeks a tl_unit from the stack
tl_unit peek(tl_stack* stack_arg)
{
	if (stack_arg->index == 0)
	{
		return stack_arg->stack[stack_arg->index];
	}
	else
	{
		return stack_arg->stack[stack_arg->index - 1];
	}
}

int get_index(tl_stack* stack_arg)
{
	return stack_arg->index;
}