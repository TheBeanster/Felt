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



typedef struct Flt_StatementBody
{
	Flt_List statements;
} Flt_StatementBody;



typedef struct Flt_Statement
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
			Flt_StatementBody* body_ontrue;
			Flt_StatementBody* body_onfalse;
		} stmt_if;
		struct
		{
			Flt_ExprNode* init;
			Flt_ExprNode* condition;
			Flt_ExprNode* loop;
			Flt_StatementBody* body;
		} stmt_for;
		struct
		{
			Flt_ExprNode* condition;
			Flt_StatementBody* body;
		} stmt_while;
		struct
		{
			Flt_ExprNode* condition;
			Flt_StatementBody* body;
		} stmt_dowhile;
		struct
		{
			Flt_StatementBody* body;
		} stmt_scope;
	};
} Flt_Statement;



void Flt_DestroyExpression(Flt_ExprNode* expr);
void Flt_DestroyStatement(Flt_Statement* stmt);
void Flt_DestroyStatementBody(Flt_StatementBody* block);
