#include "flt_parse.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fltu_memory.h"

#include "flt_code.h"



static FltE_ExprNode* read_numberliteral_node(
	const Flt_Token* token
)
{
	FltE_ExprNode* node = Flt_ALLOC_TYPE(FltE_ExprNode);
	node->type = FltE_ET_NUMBERLITERAL;
	node->num = strtod(token->string, NULL);
	return node;
}



static FltE_ExprNode* parse_expression(
	const Flt_Token* exprbegin,
	Flt_Token** exprend
)
{
	printf("Parsing expression\n");

	Flt_Token* iterator = exprbegin;
	while (iterator)
	{
		switch (iterator->type)
		{
		case Flt_TT_KEYWORD:

			break;

		case Flt_TT_SEPARATOR:

			break;
		}

		iterator = iterator->next;
	}

on_expr_end:
	printf("Expression end on ");
	Flt_PrintToken(iterator);
	printf("\n");
	*exprend = iterator;
	return NULL;
}





static FltT_StatementNode* parse_if_statement(
	const Flt_Token* tokenbegin,
	Flt_Token** nextstatement
)
{
	FltT_StatementNode* node = Flt_ALLOC_TYPE(FltT_StatementNode);
	node->type = FltT_ST_IF;

	Flt_Token* iterator = tokenbegin;
	FltE_ExprNode* condition = parse_expression(iterator, &iterator);
	node->stmt_if.condition = condition;
	
	if (iterator->keywordid != Flt_KW_THEN) // If token after condition isn't 'then'
	{
		printf("If statement condition expression doesn't end with the 'then' keyword\n");
	}
	
	

	*nextstatement = NULL;
	return NULL;
}



static FltT_StatementNode* parse_statement(
	const Flt_Token* tokenbegin,
	Flt_Token** nextstatement
)
{
	printf("Parsing statement starting with '");
	Flt_PrintToken(tokenbegin);
	printf("'\n");

	switch (tokenbegin->keywordid)
	{
	case Flt_KW_IF: return parse_if_statement(tokenbegin, nextstatement); break;

	default:
		// If statement doesn't start with a keyword then it is an expression
		break;
	}

	*nextstatement = NULL;
	return NULL;
}



static FltT_StatementBlock* parse_statementblock(
	const Flt_Token* tokenbegin
)
{
	Flt_Token* iterator = tokenbegin;
	while (iterator)
	{
		parse_statement(iterator, &iterator);
	}
}











static FltE_ExprNode* create_debug_expr_assignment()
{
	FltE_ExprNode* node = Flt_ALLOC_TYPE(FltE_ExprNode);
	node->type = FltE_ET_OPERATOR;
	node->op.id = Flt_OP_ASSIGN;
	FltE_ExprNode* nodel = Flt_ALLOC_TYPE(FltE_ExprNode);
	nodel->type = FltE_ET_VARIABLE;
	nodel->variable = "varname";

	FltE_ExprNode* noder = Flt_ALLOC_TYPE(FltE_ExprNode);
	noder->type = FltE_ET_OPERATOR;
	noder->op.id = Flt_OP_ADDITION;

	FltE_ExprNode* noderl = Flt_ALLOC_TYPE(FltE_ExprNode);
	noderl->type = FltE_ET_NUMBERLITERAL;
	noderl->num = 5;
	FltE_ExprNode* noderr = Flt_ALLOC_TYPE(FltE_ExprNode);
	noderr->type = FltE_ET_NUMBERLITERAL;
	noderr->num = 7;
	noder->op.left = noderl;
	noder->op.right = noderr;

	node->op.left = nodel;
	node->op.right = noder;
	return node;
}

static FltT_StatementBlock* create_debug_statementblock()
{
	FltT_StatementBlock* block = Flt_ALLOC_TYPE(FltT_StatementBlock);
	
	FltT_StatementNode* node1 = Flt_ALLOC_TYPE(FltT_StatementNode);
	node1->type = FltT_ST_EXPRESSION;
	node1->stmt_expr.expression = create_debug_expr_assignment();
	FltT_StatementNode* node2 = Flt_ALLOC_TYPE(FltT_StatementNode);
	node2->type = FltT_ST_IF;
	node2->stmt_if.condition = create_debug_expr_assignment();

	Flt_PushBackList(&block->statements, node1);
	Flt_PushBackList(&block->statements, node2);
}





FltT_StatementBlock* Flt_ParseSourceCode(const char* sourcecode)
{
	Flt_List tokens = { 0 };
	if (!Flt_ParseSourcecodeTokens(&tokens, sourcecode))
	{
		printf("Could not parse sourcecode!\n");
		return NULL;
	}

	for (Flt_Token* i = tokens.begin; i != NULL; i = i->next)
	{
		Flt_PrintTokenString(i);
		printf(" ");
	}
	printf("\n");

	FltT_StatementBlock* block = parse_statementblock(tokens.begin);

	return block;
}


















static void print_indent(int depth)
{
	for (int i = 0; i < depth; i++)
	{
		printf("  ");
	}
}
#define indent print_indent(depth)



static void print_exprnode(const FltE_ExprNode* expr, int depth)
{
	if (!expr) return;

	switch (expr->type)
	{
	case FltE_ET_OPERATOR:
		indent; printf("%s: {\n", flt_operatorid_names[expr->op.id]);
		print_exprnode(expr->op.left, depth + 1);
		print_exprnode(expr->op.right, depth + 1);
		indent; printf("}\n");
		break;

	case FltE_ET_NUMBERLITERAL: indent; printf("%f\n", expr->num); break;
	case FltE_ET_STRINGLITERAL: indent; printf("\"%s\"\n", expr->str); break;
	case FltE_ET_VARIABLE:		indent; printf("%s\n", expr->variable); break;

	default:
		break;
	}
}

static void print_expression(const FltE_ExprNode* expr, const char* name, int depth)
{
	indent; printf("%s: {\n", name);
	print_exprnode(expr, depth + 1);
	indent; printf("}\n");
}



static void print_statementblock(const FltT_StatementBlock* block, const char* name, int depth);

static void print_statement(const FltT_StatementNode* stmt, int depth)
{
	indent; printf("%s: {\n", flt_statementnodetype_names[stmt->type]);
	switch (stmt->type)
	{
	case FltT_ST_EXPRESSION:
		print_expression(stmt->stmt_expr.expression, "expr", depth + 1);
		break;

	case FltT_ST_IF:
		print_expression(stmt->stmt_if.condition, "condition", depth + 1);
		print_statementblock(stmt->stmt_if.body_ontrue, "ontrue", depth + 1);
		print_statementblock(stmt->stmt_if.body_onfalse, "onfalse", depth + 1);
		break;

	default:
		break;
	}
	indent; printf("}\n");
}

static void print_statementblock(const FltT_StatementBlock* block, const char* name, int depth)
{
	if (!block) return;

	indent; printf("%s: {\n", name);
	FltT_StatementNode* iterator = block->statements.begin;
	while (iterator)
	{
		print_statement(iterator, depth + 1);
		iterator = iterator->next;
	}
	indent; printf("}\n");
}



void Flt_PrintCodeTree(const FltT_StatementBlock* code)
{
	printf("code: {\n");
	print_statementblock(code, "top", 1);
	printf("}\n");
}

