#pragma once

#include "fltu_stdinc.h"

#include "flt_parse.h"
#include "flt_function.h"





typedef char Flt_ExprNodeType;
enum
{
	Flt_ET_OPERATOR,
	Flt_ET_NUMBERLITERAL,
	Flt_ET_STRINGLITERAL,
	Flt_ET_BOOLLITERAL,
	Flt_ET_OBJECT,
	Flt_ET_VARIABLE,
	Flt_ET_CLOSURE,
	Flt_ET_FUNCTIONCALL
	// TODO It might be possible to have a kind of 'ET_REFRENCE' or something for variable assignments
};

typedef struct Flt_ExprNode
{
	Flt_ExprNodeType type;
	
	union
	{
		struct
		{
			Flt_OperatorID id;
			struct Flt_ExprNode* left;
			struct Flt_ExprNode* right;
		} op;
		double num;
		Flt_Bool boolean;
		struct
		{
			char* chars;
			int len;
		} str;
		Flt_Function* closure;
		char* identifier;
		struct
		{
			char* identifier;
			int numargs;
			char** argnames;
		} functioncall;
	};
} Flt_ExprNode;

