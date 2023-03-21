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



typedef struct Flt_StatementBlock
{
	Flt_List statements;
} Flt_StatementBlock;



typedef struct FltT_StatementNode
{
	Flt_ListLinksHeader(FltT_StatementNodeBase);
	FltT_StatementNodeType type;
	union
	{
		struct
		{
			Flt_ExprNode* expression;
		} stmt_expr;
		struct
		{
			Flt_ExprNode* condition;
			Flt_StatementBlock* body_ontrue;
			Flt_StatementBlock* body_onfalse;
		} stmt_if;
		struct
		{
			Flt_ExprNode* init;
			Flt_ExprNode* condition;
			Flt_ExprNode* loop;
			Flt_StatementBlock* body;
		} stmt_for;
		struct
		{
			Flt_ExprNode* condition;
			Flt_StatementBlock* body;
		} stmt_while;
		struct
		{
			Flt_ExprNode* condition;
			Flt_StatementBlock* body;
		} stmt_dowhile;
		struct
		{
			Flt_StatementBlock* body;
		} stmt_scope;
	};
} FltT_StatementNode;

