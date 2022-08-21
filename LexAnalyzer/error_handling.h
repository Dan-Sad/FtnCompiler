#pragma once

#include <stdio.h>
#include "lex_analyzer.h"

#define LEX_ERROR -1
#define PARSE_ERROR -2

/*
	LINE ¹ maybe?
*/

// Lex error
void unidentified_token_err();

// Parse error
void missing_token_err(const char* missing_token);

// Parse variable error
void var_not_declared(const char* variable_name);