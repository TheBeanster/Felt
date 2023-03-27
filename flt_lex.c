#include "flt_parse.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "fltu_memory.h"
#include "fltu_string.h"



typedef enum
{
	CT_NULL,		// Default type or any char not recognised
	CT_ALPHA,		// Letter or underscore
	CT_NUMBER,
	CT_POINT,
	CT_OPERATOR,
	CT_SEPARATOR,
	CT_SPACER,		// Space or tab
	CT_ENDLINE,
	CT_QUOTE,		// Quotation marks for strings
	CT_COMMENT,		// Comments start with # and end with an endline
} chartype;

static chartype check_chartype(const char c)
{
	if (isalpha(c) || c == '_')		return CT_ALPHA;
	if (isdigit(c))					return CT_NUMBER;
	if (c == '.')					return CT_POINT;
	if (strchr("+-/*=<>!&|^", c))	return CT_OPERATOR;
	if (strchr("()[]{},", c))		return CT_SEPARATOR;
	if (isblank(c))					return CT_SPACER;
	if (c == '\n' || c == ';')		return CT_ENDLINE; // Semicolon acts the same as endline
	if (c == '\"' || c == '\'')		return CT_QUOTE;
	if (c == '#')					return CT_COMMENT;
	return CT_NULL;
}





static Flt_Token* create_token()
{
	Flt_Token* token = Flt_MALLOC(sizeof(Flt_Token));
	token->prev = token->next = NULL;
	token->type = Flt_TT_NULL;
	token->keywordid = Flt_KW_NULL;
	token->operatorid = Flt_OP_NULL;
	token->separatorid = Flt_SP_NULL;
	token->string = NULL;
}



static void push_alpha_token(
	Flt_List* tokens,
	const char* sourcecode,
	int start,
	int end
)
{
	Flt_Token* token = create_token();

	char* cutstring = Flt_CopyCutString(sourcecode, start, end - start);
	token->keywordid = Flt_GetKeyword(cutstring);
	if (token->keywordid == Flt_KW_NULL)
	{
		token->type = Flt_TT_IDENTIFIER;
		token->string = cutstring;
	} else
	{
		token->type = Flt_TT_KEYWORD;
		token->string = NULL;
		Flt_FREE(cutstring);
	}

	Flt_PushBackList(tokens, token);
}



static void push_numberliteral_token(
	Flt_List* tokens,
	const char* sourcecode,
	int start,
	int end
)
{
	Flt_Token* token = create_token();
	token->type = Flt_TT_NUMBERLITERAL;

	token->string = Flt_CopyCutString(sourcecode, start, end - start);

	Flt_PushBackList(tokens, token);
}



static Flt_Bool push_operator_token(
	Flt_List* tokens,
	const char* sourcecode,
	int start,
	int end
)
{
	Flt_Token* token = Flt_MALLOC(sizeof(Flt_Token));
	token->prev = token->next = NULL;
	token->type = Flt_TT_OPERATOR;
	token->keywordid = Flt_KW_NULL;
	token->separatorid = Flt_SP_NULL;
	token->string = NULL;

	char* cutstring = Flt_CopyCutString(sourcecode, start, end - start);
	token->operatorid = Flt_GetOperator(cutstring);
	Flt_FREE(cutstring);

	if (token->operatorid == Flt_OP_NULL)
	{
		Flt_FREE(token);
		return Flt_FALSE;
	}

	Flt_PushBackList(tokens, token);
	return Flt_TRUE;
}



static Flt_Bool push_separator_token(
	Flt_List* tokens,
	const char* sourcecode,
	int start
)
{
	Flt_Token* token = create_token();
	token->type = Flt_TT_SEPARATOR;

	token->separatorid = Flt_GetSeparator(sourcecode[start]);
	if (token->separatorid == Flt_SP_NULL)
	{
		Flt_FREE(token);
		return Flt_FALSE;
	}

	Flt_PushBackList(tokens, token);
	return Flt_TRUE;
}



static void push_stringliteral_token(
	Flt_List* tokens,
	const char* sourcecode,
	int start,
	int end
)
{
	Flt_Token* token = create_token();
	token->type = Flt_TT_STRINGLITERAL;
	token->string = Flt_CopyCutString(sourcecode, start + 1, end - 1); // Don't include quotation marks
	Flt_PushBackList(tokens, token);
}





/// @brief Reads through a string character by character and divides it into separate tokens.
/// @param tokens Pointer to an empty list to put the tokens into.
/// @param sourcecode Pointer to a string.
/// @return Flt_TRUE if the sourcecode parsed succesfully, Flt_FALSE if not.
Flt_Bool Flt_ParseSourcecodeTokens(Flt_List* tokens, const char* sourcecode)
{
	// Reading tokens works by checking where a token starts and where it ends, character by character,
	// by checking what the type of the current token is and how that token type would end.
	// For example, a text token ends when a char that isn't a letter, underscore or number apears, such as an operator char.
	// It will then push that token to the 'tokens' list, set the tokenchange flag and start reading the new token.

	int	linenum = 1;

	// Where the current token being read starts in the source buffer.
	int	curtoken_begin = 0;

	// The chartype of the current token being read. 
	// This determines how it will check for token endings.
	// Not that it is not a Flt_TokenType, since they can't be used when reading tokens,
	// since things like keywords and identifiers use the same code for being read.
	chartype curtoken_type = CT_SPACER;
	// Starts as spacer since that will check for new tokens.

	Flt_Bool tokenchange = Flt_FALSE;

	// When reading a numberliteral, check if a decimal point has been found.
	// If this is true and you find another decimal point, that will be an error.
	Flt_Bool numberliteral_decimal = Flt_FALSE;



	int i = 0;
	while(1)
	{
		// Looping over every char in the sourcecode one by one

		char		c = sourcecode[i];
		chartype	ctype = check_chartype(c);



		// Depending on the type of the token currently being read, 
		// each block contains code determining if the token should change.
		switch (curtoken_type)
		{
		case CT_ALPHA:
			if (!(ctype == CT_ALPHA || ctype == CT_NUMBER))
			{
				// Text token ends when a character that isn't alpha or number appears
				push_alpha_token(tokens, sourcecode, curtoken_begin, i);
				tokenchange = Flt_TRUE;
			}
			break;



		case CT_NUMBER:
			if (numberliteral_decimal)
			{
				if (ctype == CT_POINT)
				{
					// Two decimal points in one number error
					printf("Two decimal points in one number!\n");
					goto onerror;
				}
			} else if (ctype == CT_POINT)
				numberliteral_decimal = Flt_TRUE;

			if (!(ctype == CT_NUMBER || ctype == CT_POINT))
			{
				if (ctype == CT_ALPHA)
				{
					// A letter or underscore directly after number
					printf("A letter or underscore directly after number!\n");
					goto onerror;
				}
				push_numberliteral_token(tokens, sourcecode, curtoken_begin, i);
				tokenchange = Flt_TRUE;
				numberliteral_decimal = Flt_FALSE;
			}
			break;

		case CT_POINT:
			if (curtoken_type == CT_NUMBER)
			{
				curtoken_type = CT_NUMBER;
			} else
			{
				if (!push_operator_token(tokens, sourcecode, curtoken_begin, i)) goto onerror;
				tokenchange = Flt_TRUE;
			}
			break;



		case CT_OPERATOR:
			if (ctype != CT_OPERATOR)
			{
				// Operator ends when non operator token appears
				if (!push_operator_token(tokens, sourcecode, curtoken_begin, i)) goto onerror;
				tokenchange = Flt_TRUE;
			}
			break;



		case CT_SEPARATOR:
			// Separators are always 1 character so push it immedietly
			if (!push_separator_token(tokens, sourcecode, curtoken_begin)) goto onerror;
			tokenchange = Flt_TRUE;
			break;



		case CT_QUOTE:
			if (ctype == CT_ENDLINE)
			{
				printf("String doesn't have closing quotes!");
				goto onerror;
			}
			// String literals end when another double quote is found
			if (ctype == CT_QUOTE)
			{
				push_stringliteral_token(tokens, sourcecode, curtoken_begin, i + 1);
				tokenchange = Flt_TRUE;
			}
			break;



		case CT_COMMENT:
			// Comments end with endlines
			if (ctype == CT_ENDLINE)
				tokenchange = Flt_TRUE;
			break;

			// Default is error, it should be considered spacer
		default:
			// TODO Add warning about invalid characters
		case CT_SPACER:
			if (ctype != CT_SPACER)
				tokenchange = Flt_TRUE;
			break;
		}



		if (tokenchange)
		{
			// After a token end, this will determine what the new token is.

			if ((curtoken_type == CT_QUOTE) || (curtoken_type == CT_COMMENT))
			{
				// Special cases for string and comment since they both stop at specific characters.
				// A double quotation mark for strings and endline for comments.
				// Set the curtoken_type as a spacer so the next type will be read properly.
				curtoken_type = CT_SPACER;
			} else
			{
				// Set the new tokens start position as the current iterator position.
				curtoken_begin = i;
				curtoken_type = ctype;
			}
			tokenchange = Flt_FALSE;
		}

		if (c == '\0') break;
		i++; // Next char
	}



	// Check if the current type is a string liteFlt
	if (curtoken_type == CT_QUOTE)
	{
		printf("String doesn't have closing quotes!\n");
		goto onerror;
	}

	return Flt_TRUE;

onerror:
	Flt_ClearList(tokens, &Flt_DestroyToken);
	return Flt_FALSE;
}
