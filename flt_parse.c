#include "flt_parse.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "fltu_memory.h"
#include "fltu_string.h"

#include "flt_code.h"



static FltE_ExprNode* parse_expression(
	const Flt_Token* tokenbegin,
	Flt_Token** expressionend
)
{
	printf("Parsing expression\n");

	Flt_Token* iterator = tokenbegin;
	while (iterator)
	{
		

		iterator = iterator->next;
	}

	*expressionend = tokenbegin->next->next;
	return NULL;
}





static FltT_StatementNode* parse_if_statement(
	const Flt_Token* tokenbegin,
	Flt_Token** nextstatement
)
{
	FltT_StatementNode* node = Flt_ALLOC_TYPE(FltT_StatementNode);
	node->type = FltT_ST_IF;

	Flt_Token* iterator = tokenbegin;
	FltE_ExprNode* condition = parse_expression(iterator, &iterator);
	node->stmt_if.condition = condition;
	
	if (iterator->keywordid != Flt_KW_THEN) // If token after condition isn't 'then'
	{
		printf("If statement condition expression doesn't end with the 'then' keyword\n");
		printf("AAAAAAAAAAAAA\n");
	}
	
	

	*nextstatement = NULL;
	return NULL;
}



static FltT_StatementNode* parse_statement(
	const Flt_Token* tokenbegin,
	Flt_Token** nextstatement
)
{
	printf("Parsing statement starting with '");
	Flt_PrintToken(tokenbegin);
	printf("'\n");

	switch (tokenbegin->keywordid)
	{
	case Flt_KW_IF: return parse_if_statement(tokenbegin, nextstatement); break;

	default:
		// If statement doesn't start with a keyword then it is an expression
		break;
	}

	*nextstatement = NULL;
	return NULL;
}



static FltT_StatementBlock* parse_statementblock(
	const Flt_Token* tokenbegin
)
{
	Flt_Token* iterator = tokenbegin;
	while (iterator)
	{
		parse_statement(iterator, &iterator);
	}
}





FltT_StatementBlock* Flt_ParseSourceCode(const char* sourcecode)
{
	Flt_List tokens = { 0 };
	if (!Flt_ParseSourcecodeTokens(&tokens, sourcecode))
	{
		printf("Could not parse sourcecode!\n");
		return NULL;
	}

	for (Flt_Token* i = tokens.begin; i != NULL; i = i->next)
	{
		Flt_PrintToken(i);
		printf(" ");
	}
	printf("\n");

	parse_statementblock(tokens.begin);

	return NULL;
}

