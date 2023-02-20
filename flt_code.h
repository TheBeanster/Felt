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
	FltT_ST_SCOPE,
	FltT_NUM_STATEMENTTYPES
} FltT_StatementNodeType;

extern const char* flt_statementnodetype_names[FltT_NUM_STATEMENTTYPES];



typedef struct FltT_StatementBlock
{
	Flt_List statements;
} FltT_StatementBlock;



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
			FltT_StatementBlock* body_ontrue;
			FltT_StatementBlock* body_onfalse;
		} stmt_if;
		struct
		{
			FltE_ExprNode* init;
			FltE_ExprNode* condition;
			FltE_ExprNode* loop;
			FltT_StatementBlock* body;
		} stmt_for;
		struct
		{
			FltE_ExprNode* condition;
			FltT_StatementBlock* body;
		} stmt_while;
		struct
		{
			FltE_ExprNode* condition;
			FltT_StatementBlock* body;
		} stmt_dowhile;
		struct
		{
			FltT_StatementBlock* body;
		} stmt_scope;
	};
} FltT_StatementNode;

