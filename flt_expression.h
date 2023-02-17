#pragma once

#include "fltu_stdinc.h"

#include "flt_parse.h"





typedef char FltE_ExprNodeType;
enum
{
	FltE_ET_OPERATOR,
	FltE_ET_NUMBERLITERAL,
	FltE_ET_STRINGLITERAL,
	FltE_ET_LAMBDA,
	FltE_ET_VARIABLE,
	FltE_ET_FUNCTIONCALL
};

typedef struct FltE_ExprNode
{
	struct FltE_ExprNode* left;
	struct FltE_ExprNode* right;
	FltE_ExprNodeType type;

	union
	{
		Flt_OperatorID op;
		double num;
		char* str;
	};
} FltE_ExprNode;

