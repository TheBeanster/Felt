#pragma once

#include "fltu_stdinc.h"



typedef enum
{
	FltE_ET_OPERATOR,
	FltE_ET_NUMBERLITERAL,
	FltE_ET_STRINGLITERAL,
	FltE_ET_VARIABLE,
	FltE_ET_FUNCTIONCALL
} FltE_ExprNodeType;

typedef struct FltE_ExprNode
{
	struct FltE_ExprNode* left;
	struct FltE_ExprNode* right;
	FltE_ExprNodeType type;
} FltE_ExprNode;



