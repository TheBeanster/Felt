#include "flt_parse.h"

#include <string.h>
#include <stdio.h>

#include "fltu_memory.h"



const char* flt_keyword_strings[Flt_NUM_KEYWORDS] =
{
	"if",
	"then",
	"else",
	"for",
	"do",
	"while",

	"function",
	"return",

	"false",
	"true",

	"end",
};

const char* flt_keywordid_names[Flt_NUM_KEYWORDS] =
{
	"KW_IF",
	"KW_THEN",
	"KW_ELSE",
	"KW_FOR",
	"KW_DO",
	"KW_WHILE",
	
	"KW_FUNCTION",
	"KW_RETURN",
	
	"KW_FALSE",
	"KW_TRUE",
	
	"KW_END",
};

Flt_KeywordID Flt_GetKeyword(const char* string)
{
	for (int i = 0; i < Flt_NUM_KEYWORDS; i++)
	{
		if (strcmp(string, flt_keyword_strings[i]) == 0)
			return i;
	}
	return Flt_KW_NULL;
}



const char* flt_operator_strings[Flt_NUM_OPERATORS] =
{
	"=",
	"+=",
	"-=",
	"*=",
	"/=",

	"!",
	"", // Unary negative should not be set by finding it here

	"==",
	"!=",
	"<",
	">",
	"<=",
	">=",

	"+",
	"-",
	"*",
	"/",
	"%",

	"."
};

const char* flt_operatorid_names[Flt_NUM_OPERATORS] =
{
	"OP_ASSIGN",
	"OP_ASSIGNADDITION",
	"OP_ASSIGNSUBTRACTION",
	"OP_ASSIGNMULTIPLICATION",
	"OP_ASSIGNDIVISION",
	
	"OP_NOT",
	"OP_AND",
	"OP_OR",
	"OP_XOR",
	"OP_NEGATIVE",
	
	"OP_EQUALITY",
	"OP_INEQUALITY",
	"OP_LESS",
	"OP_GREATER",
	"OP_LESSEQUAL",
	"OP_GREATEREQUAL",
	
	"OP_ADDITION",
	"OP_SUBTRACTION",
	"OP_MULTIPLICATION",
	"OP_DIVISION",
	
	"OP_MEMBERACCESS",
};

Flt_OperatorID Flt_GetOperator(const char* string)
{
	for (int i = 0; i < Flt_NUM_OPERATORS; i++)
	{
		if (strcmp(string, flt_operator_strings[i]) == 0)
			return i;
	}
	return Flt_OP_NULL;
}



const char flt_separator_chars[Flt_NUM_SEPARATORS] =
{
	';',
	',',
	'(',
	')',
	'[',
	']',
	'{',
	'}',
};

const char* flt_separatorid_names[Flt_NUM_SEPARATORS] =
{
	"SP_SEMICOLON",
	"SP_COMMA",
	"SP_LPAREN",
	"SP_RPAREN",
	"SP_LBRACKET",
	"SP_RBRACKET",
	"SP_LBRACE",
	"SP_RBRACE",
};

Flt_SeparatorID Flt_GetSeparator(char c)
{
	for (int i = 0; i < Flt_NUM_SEPARATORS; i++)
	{
		if (c == flt_separator_chars[i])
			return i;
	}
	return Flt_SP_NULL;
}





void Flt_PrintToken(const Flt_Token* token)
{
	if (!token) return;

	switch (token->type)
	{
	case Flt_TT_KEYWORD:		printf("%s", flt_keywordid_names[token->keywordid]); break;
	case Flt_TT_OPERATOR:		printf("%s", flt_operatorid_names[token->operatorid]); break;
	case Flt_TT_SEPARATOR:		printf("%s", flt_separatorid_names[token->separatorid]); break;
	case Flt_TT_NUMBERLITERAL:	printf("%s", token->string); break;
	case Flt_TT_STRINGLITERAL:	printf("\"%s\"", token->string); break;
	case Flt_TT_IDENTIFIER:		printf("%s", token->string); break;
	default:
		printf("invalid");
		break;
	}
}
void Flt_PrintTokenString(const Flt_Token* token)
{
	if (!token) return;

	switch (token->type)
	{
	case Flt_TT_KEYWORD:		printf("%s", flt_keyword_strings[token->keywordid]); break;
	case Flt_TT_OPERATOR:		printf("%s", flt_operator_strings[token->operatorid]); break;
	case Flt_TT_SEPARATOR:		printf("%c", flt_separator_chars[token->separatorid]); break;
	case Flt_TT_NUMBERLITERAL:	printf("%s", token->string); break;
	case Flt_TT_STRINGLITERAL:	printf("\"%s\"", token->string); break;
	case Flt_TT_IDENTIFIER:		printf("%s", token->string); break;
	default:
		printf("invalid");
		break;
	}
}



void Flt_DestroyToken(Flt_Token* token)
{
	if (token->string)
		Flt_FREE(token->string);
	Flt_FREE(token);
}
