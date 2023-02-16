#include "flt_parse.h"

#include <string.h>



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

Flt_KeywordID Flt_GetKeyword(const char* string)
{
	for (int i = 0; i < Flt_NUM_KEYWORDS; i++)
	{
		if (strcmp(string, flt_keyword_strings[i]) == 0)
			return i + 1;
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

Flt_OperatorID Flt_GetOperator(const char* string)
{
	for (int i = 0; i < Flt_NUM_OPERATORS; i++)
	{
		if (strcmp(string, flt_operator_strings[i]) == 0)
			return i + 1;
	}
	return Flt_OP_NULL;
}



const char flt_separator_chars[Flt_NUM_SEPARATORS] =
{
	'(',
	')',
	'[',
	']',
	'{',
	'}',
	','
};

Flt_SeparatorID Flt_GetSeparator(char c)
{
	for (int i = 0; i < Flt_NUM_SEPARATORS; i++)
	{
		if (c == flt_separator_chars[i])
			return i + 1;
	}
	return Flt_SP_NULL;
}

