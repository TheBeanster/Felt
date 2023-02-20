#pragma once

#include "fltu_stdinc.h"
#include "fltu_list.h"



typedef char Flt_TokenType;
enum
{
	Flt_TT_NULL,
	Flt_TT_KEYWORD,
	Flt_TT_OPERATOR,
	Flt_TT_SEPARATOR,
	Flt_TT_NUMBERLITERAL,
	Flt_TT_STRINGLITERAL,
	Flt_TT_IDENTIFIER,
	Flt_TT_ENDLINE,
};



typedef char Flt_KeywordID;
enum
{
	Flt_KW_IF,
	Flt_KW_THEN,
	Flt_KW_ELSE,
	Flt_KW_FOR,
	Flt_KW_DO,
	Flt_KW_WHILE,

	Flt_KW_FUNCTION,
	Flt_KW_RETURN,

	Flt_KW_FALSE,
	Flt_KW_TRUE,

	Flt_KW_END,

	Flt_NUM_KEYWORDS,
	Flt_KW_NULL = -1, // Invalid or non keyword
};
extern const char* flt_keyword_strings[Flt_NUM_KEYWORDS];
extern const char* flt_keywordid_names[Flt_NUM_KEYWORDS];

/// Checks the keyword id of a string.
/// @param string String to check
/// @return The ID of the keyword or Flt_KW_NULL if not keyword and instead identifier
Flt_KeywordID Flt_GetKeyword(const char* string);



typedef char Flt_OperatorID;
enum
{
	Flt_OP_ASSIGN,					// '='
	Flt_OP_ASSIGNADDITION,			// '+='
	Flt_OP_ASSIGNSUBTRACTION,		// '-='
	Flt_OP_ASSIGNMULTIPLICATION,	// '*='
	Flt_OP_ASSIGNDIVISION,			// '/='

	Flt_OP_NOT,						// '!' Logical not
	Flt_OP_AND,						// '&' Logical and
	Flt_OP_OR,						// '|' Logical or
	Flt_OP_XOR,						// '^' Logical xor
	Flt_OP_NEGATIVE,				// '-' Unary negative
	
	Flt_OP_EQUALITY,				// '=='
	Flt_OP_INEQUALITY,				// '!='
	Flt_OP_LESS,					// '<'
	Flt_OP_GREATER,					// '>'
	Flt_OP_LESSEQUAL,				// '<='
	Flt_OP_GREATEREQUAL,			// '>='
	
	Flt_OP_ADDITION,				// '+' Arithmetic add
	Flt_OP_SUBTRACTION,				// '-' Arithmetic subtract
	Flt_OP_MULTIPLICATION,			// '*' Arithmetic multiply
	Flt_OP_DIVISION,				// '/' Arithmetic divide
	
	Flt_OP_MEMBERACCESS,			// '.'

	Flt_NUM_OPERATORS,
	Flt_OP_NULL = -1,				// Invalid or non operator
};
extern const char* flt_operator_strings[Flt_NUM_OPERATORS];
extern const char* flt_operatorid_names[Flt_NUM_OPERATORS];

/// Checks the operator id of a string.
/// @param string String to check
/// @return The ID of the operator or Flt_OP_NULL if not operator
Flt_OperatorID Flt_GetOperator(const char* string);

#define Flt_IsAssignmentOp(op)	((op) >= Flt_OP_ASSIGN		|| (op) <= Flt_OP_ASSIGNDIVISION)
#define Flt_IsLogicalOp(op)		((op) >= Flt_OP_NOT			|| (op) <= Flt_OP_XOR)
#define Flt_IsRelationalOp(op)	((op) >= Flt_OP_EQUALITY	|| (op) <= Flt_OP_GREATEREQUAL)
#define Flt_IsArithmeticOp(op)	((op) >= Flt_OP_ADDITION	|| (op) <= Flt_OP_DIVISION)
#define Flt_IsUnaryOp(op)		((op) == Flt_OP_NOT	|| (op) == Flt_OP_NEGATIVE)



typedef char Flt_SeparatorID;
enum
{
	Flt_SP_LPAREN,		// '('
	Flt_SP_RPAREN,		// ')'
	Flt_SP_LBRACKET,	// '['
	Flt_SP_RBRACKET,	// ']'
	Flt_SP_LBRACE,		// '{'
	Flt_SP_RBRACE,		// '}'
	Flt_SP_COMMA,		// ','

	Flt_NUM_SEPARATORS,
	Flt_SP_NULL = -1,	// Invalid or non separator
};
extern const char flt_separator_chars[Flt_NUM_SEPARATORS];
extern const char* flt_separatorid_names[Flt_NUM_SEPARATORS];

/// Checks the separator id of a char.
/// @param c Character to check
/// @return The ID of the separator or Flt_SP_NULL if not separator char
Flt_SeparatorID Flt_GetSeparator(char c);





typedef struct Flt_Token
{
	Flt_ListLinksHeader(Flt_Token);

	Flt_TokenType	type;
	Flt_KeywordID	keywordid;
	Flt_OperatorID	operatorid;
	Flt_SeparatorID separatorid;

	char* string;
} Flt_Token;

void Flt_PrintToken(const Flt_Token* token);

void Flt_PrintTokenString(const Flt_Token* token);

void Flt_DestroyToken(Flt_Token* token);



Flt_Bool Flt_ParseSourcecodeTokens(Flt_List* tokens, const char* sourcecode);

struct FltT_StatementBlock* Flt_ParseSourceCode(const char* sourcecode);



void Flt_PrintCodeTree(const struct FltT_StatementBlock* code);
