#include "error_handling.h"

#define LEX_ERROR -1
#define PARSE_ERROR -2

void unidentified_token_err()
{
	printf("Error: Unidentified token!\n");
}

void missing_token_err(const char* missing_token)
{
	printf("Error: Missing token - %s\n", missing_token);
}

void var_not_declared(const char* variable_name)
{
	printf("Error: Variable - %s not declared!\n", variable_name);
}

/*
void missing_token_err_tk(enum TOKEN_IDs missing_token)
{
	//...
}
*/