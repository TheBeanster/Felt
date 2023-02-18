#pragma once

#include "fltu_stdinc.h"

#include "flt_parse.h"
#include "flt_function.h"





typedef char FltE_ExprNodeType;
enum
{
	FltE_ET_OPERATOR,
	FltE_ET_NUMBERLITERAL,
	FltE_ET_STRINGLITERAL,
	FltE_ET_LAMBDA,
	FltE_ET_OBJECT,
	FltE_ET_VARIABLE,
	FltE_ET_FUNCTIONCALL
};

typedef struct FltE_ExprNode
{
	FltE_ExprNodeType type;
	
	union
	{
		struct
		{
			Flt_OperatorID op;
			struct FltE_ExprNode* left;
			struct FltE_ExprNode* right;
		} op;
		double num;
		struct
		{
			char* chars;
			int len;
		} str;
		Flt_Function function;
	};
} FltE_ExprNode;

