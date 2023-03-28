#pragma once

#include "fltu_stdinc.h"
#include "fltu_list.h"

#include "flt_expression.h"



typedef enum
{
	Flt_ST_EXPRESSION,
	Flt_ST_IF,
	Flt_ST_FOR,
	Flt_ST_WHILE,
	Flt_ST_DOWHILE,
	Flt_ST_SCOPE,
	Flt_NUM_STATEMENTTYPES
} Flt_StatementNodeType;

extern const char* flt_statementnodetype_names[Flt_NUM_STATEMENTTYPES];



typedef struct Flt_StatementBlock
{
	Flt_List statements;
} Flt_StatementBlock;



typedef struct Flt_StatementNode
{
	Flt_ListLinksHeader(Flt_StatementNodeBase);
	Flt_StatementNodeType type;
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
} Flt_StatementNode;



void Flt_DestroyExpression(Flt_ExprNode* expr);
void Flt_DestroyStatement(Flt_StatementNode* stmt);
void Flt_DestroyStatementBlock(Flt_StatementBlock* block);
