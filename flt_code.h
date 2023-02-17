#pragma once

#include "fltu_stdinc.h"
#include "fltu_list.h"

#include "flt_expression.h"



typedef enum
{
	FltT_ST_EXPRESSION,
	FltT_ST_IF,
	FltT_ST_FOR,
	FltT_ST_WHILE,
	FltT_ST_DOWHILE,
	FltT_ST_SCOPE
} FltT_StatementNodeType;



typedef struct FltT_CodeBody
{
	Flt_List statements;
} FltT_CodeBody;



typedef struct FltT_StatementNode
{
	Flt_ListLinksHeader(FltT_StatementNodeBase);
	FltT_StatementNodeType type;
	union
	{
		struct
		{
			FltE_ExprNode* expression;
		} stmt_expr;
		struct
		{
			FltE_ExprNode* condition;
			FltT_CodeBody* body_ontrue;
			FltT_CodeBody* body_onfalse;
		} stmt_if;
		struct
		{
			FltE_ExprNode* init;
			FltE_ExprNode* condition;
			FltE_ExprNode* loop;
			FltT_CodeBody* body;
		} stmt_for;
		struct
		{
			FltE_ExprNode* condition;
			FltT_CodeBody* body;
		} stmt_while;
		struct
		{
			FltE_ExprNode* condition;
			FltT_CodeBody* body;
		} stmt_dowhile;
		struct
		{
			FltT_CodeBody* body;
		} stmt_scope;
	};
} FltT_StatementNode;

