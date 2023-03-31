#include "flt_parse.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fltu_memory.h"

#include "flt_code.h"



static Flt_ExprNode* create_boolkw_node(
	const Flt_Bool b
)
{
	Flt_ExprNode* node = Flt_ALLOC_TYPE(Flt_ExprNode);
	node->type = Flt_ET_BOOLLITERAL;
	node->boolean = b;
	return node;
}



static Flt_ExprNode* parse_numberliteral_node(
	const Flt_Token* token
)
{
	Flt_ExprNode* node = Flt_ALLOC_TYPE(Flt_ExprNode);
	node->type = Flt_ET_NUMBERLITERAL;
	node->num = strtod(token->string, NULL);
	return node;
}



static Flt_ExprNode* parse_stringliteral_node(
	const Flt_Token* token
)
{
	Flt_ExprNode* node = Flt_ALLOC_TYPE(Flt_ExprNode);
	node->type = Flt_ET_STRINGLITERAL;
	node->str.chars = _strdup(token->string);
	node->str.len = strlen(token->string);
	return node;
}



static Flt_ExprNode* parse_operator_node(
	const Flt_Token* token
)
{
	Flt_ExprNode* node = Flt_ALLOC_TYPE(Flt_ExprNode);
	node->type = Flt_ET_OPERATOR;
	node->op.id = token->operatorid;
	return node;
}



static Flt_ExprNode* parse_expression(
	const Flt_Token* tokenbegin,
	Flt_Token** exprend
)
{
	printf("Parsing expression\n");

	Flt_ExprNode* expr = Flt_ALLOC_TYPE(Flt_ExprNode);

	Flt_List l_operators = { 0 };
	Flt_List l_operands = { 0 };

	Flt_Token* iterator = tokenbegin;
	while (iterator)
	{
		switch (iterator->type)
		{

		case Flt_TT_KEYWORD:
			if (!((iterator->keywordid == Flt_KW_TRUE) || (iterator->keywordid == Flt_KW_FALSE)))
			{
				if (iterator->keywordid == Flt_KW_FUNCTION)
				{
					printf("Closure!!!\n");
				} else
					goto on_expr_end;
			}
			Flt_PushBackList(&l_operands, create_boolkw_node(iterator->keywordid == Flt_KW_TRUE));
			break;

		case Flt_TT_NUMBERLITERAL:
			Flt_PushBackList(&l_operands, parse_numberliteral_node(iterator));
			break;

		case Flt_TT_STRINGLITERAL:
			Flt_PushBackList(&l_operands, parse_stringliteral_node(iterator));
			break;


		case Flt_TT_SEPARATOR:
			if (iterator->separatorid == Flt_SP_LPAREN) // Subexpression
			{
				if (iterator->next == NULL)
				{
					printf("Code ends after '('\n");
					goto on_error;
				}

				Flt_ExprNode* subexpr = parse_expression(iterator->next, &iterator);
				if (!subexpr) goto on_error;
				if (iterator->separatorid != Flt_SP_RPAREN)
				{
					printf("Mismatching token '");
					Flt_PrintToken(iterator);
					printf("', expected ')'\n");
					goto on_error;
				}
				
				Flt_PushBackList(&l_operands, subexpr);
			} else if (iterator->separatorid == Flt_SP_LBRACE) // Object definition
			{

			} else if (iterator->separatorid == Flt_SP_LBRACKET) // Array definition
			{

			} else
				goto on_expr_end; // All other separators end the expression
			break;


		case Flt_TT_IDENTIFIER:
			
			break;



		case Flt_TT_OPERATOR:
			Flt_PushBackList(&l_operators, parse_operator_node(iterator));
			break;



		default:
			printf("Invalid expression token\n");
			goto on_error;
		}

		iterator = iterator->next;
	}

on_expr_end:
	printf("Expression end on ");
	Flt_PrintToken(iterator);
	printf("\n");

	*exprend = iterator;

	printf("Expression operators = ");
	iterator = l_operators.begin;
	while (iterator)
	{
		printf("   Op:%s   ", flt_operatorid_names[iterator->operatorid]);
		iterator = iterator->next;
	}
	printf("\n");

	return expr;

on_error:
	Flt_FREE(expr);
	return NULL;
}




static Flt_StatementNode* parse_expression_statement(
	const Flt_Token* tokenbegin,
	Flt_Token** nextstatement
)
{
	Flt_StatementNode* node = Flt_ALLOC_TYPE(Flt_StatementNode);
	node->type = Flt_ST_EXPRESSION;
	node->stmt_expr.expression = parse_expression(tokenbegin, nextstatement);
	if (!node->stmt_expr.expression)
	{
		printf("Couldn't parse expression statement\n");
		Flt_FREE(node);
		return NULL;
	}
	return node;
}




static Flt_StatementNode* parse_if_statement(
	const Flt_Token* tokenbegin,
	Flt_Token** nextstatement
)
{
	// Make sure token after 'if' is ( and that there is a token after that too
	if (tokenbegin->next && tokenbegin->next->separatorid != Flt_SP_LPAREN && tokenbegin->next->next)
	{
		printf("Expected '(' after if keyword\n");
		return NULL;
	}

	// Parse the condition expression
	Flt_Token* conditiontoken = tokenbegin->next->next;
	Flt_ExprNode* condition = parse_expression(conditiontoken, &conditiontoken);
	
	if (condition == NULL || conditiontoken->separatorid != Flt_SP_RPAREN) // If condition doesn't end with ')'
	{
		printf("If statement condition expression doesn't end with a ')'\n");
		Flt_DestroyExpression(condition);
		return NULL;
	}

	Flt_StatementNode* node = Flt_ALLOC_TYPE(Flt_StatementNode);
	node->type = Flt_ST_IF;
	
	node->stmt_if.condition = condition;

	*nextstatement = conditiontoken->next;
	return node;
}



static Flt_StatementNode* parse_statement(
	const Flt_Token* tokenbegin,
	Flt_Token** nextstatement
)
{
	printf("Parsing statement starting with '");
	Flt_PrintToken(tokenbegin);
	printf("'\n");

	Flt_StatementNode* node = NULL;

	switch (tokenbegin->keywordid)
	{
	case Flt_KW_IF: 
		node = parse_if_statement(tokenbegin, nextstatement);
		if (!node)
		{
			printf("Failed to parse if statement\n");
			return NULL;
		}
		return node;

	default:
		// If statement doesn't start with a keyword then it is an expression
		parse_expression_statement(tokenbegin, nextstatement);
		break;
	}

	*nextstatement = NULL;
	return NULL;
}



static Flt_StatementBlock* parse_statementblock(
	const Flt_Token* tokenbegin
)
{
	Flt_StatementBlock* block = Flt_ALLOC_TYPE(Flt_StatementBlock);
	
	Flt_Token* iterator = tokenbegin;
	while (iterator)
	{
		Flt_StatementNode* node = parse_statement(iterator, &iterator);
		if (!node) goto on_fail;
		Flt_PushBackList(&block->statements, node);
	}

	return block;

on_fail:
	Flt_ClearList(&block->statements, &Flt_DestroyStatement);
	Flt_FREE(block);
	return NULL;
}











static Flt_ExprNode* create_debug_expr_assignment()
{
	Flt_ExprNode* node = Flt_ALLOC_TYPE(Flt_ExprNode);
	node->type = Flt_ET_OPERATOR;
	node->op.id = Flt_OP_ASSIGN;
	Flt_ExprNode* nodel = Flt_ALLOC_TYPE(Flt_ExprNode);
	nodel->type = Flt_ET_VARIABLE;
	nodel->identifier = "varname";

	Flt_ExprNode* noder = Flt_ALLOC_TYPE(Flt_ExprNode);
	noder->type = Flt_ET_OPERATOR;
	noder->op.id = Flt_OP_ADDITION;

	Flt_ExprNode* noderl = Flt_ALLOC_TYPE(Flt_ExprNode);
	noderl->type = Flt_ET_NUMBERLITERAL;
	noderl->num = 5;
	Flt_ExprNode* noderr = Flt_ALLOC_TYPE(Flt_ExprNode);
	noderr->type = Flt_ET_NUMBERLITERAL;
	noderr->num = 7;
	noder->op.left = noderl;
	noder->op.right = noderr;

	node->op.left = nodel;
	node->op.right = noder;
	return node;
}

static Flt_StatementBlock* create_debug_statementblock()
{
	Flt_StatementBlock* block = Flt_ALLOC_TYPE(Flt_StatementBlock);
	
	Flt_StatementNode* node1 = Flt_ALLOC_TYPE(Flt_StatementNode);
	node1->type = Flt_ST_EXPRESSION;
	node1->stmt_expr.expression = create_debug_expr_assignment();
	Flt_StatementNode* node2 = Flt_ALLOC_TYPE(Flt_StatementNode);
	node2->type = Flt_ST_IF;
	node2->stmt_if.condition = create_debug_expr_assignment();

	Flt_PushBackList(&block->statements, node1);
	Flt_PushBackList(&block->statements, node2);
}





Flt_StatementBlock* Flt_ParseSourceCode(const char* sourcecode)
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

	Flt_StatementBlock* block = parse_statementblock(tokens.begin);

	return NULL;
}


















static void print_indent(int depth)
{
	for (int i = 0; i < depth; i++)
	{
		printf("  ");
	}
}
#define indent print_indent(depth)



static void print_exprnode(const Flt_ExprNode* expr, int depth)
{
	if (!expr) return;

	switch (expr->type)
	{
	case Flt_ET_OPERATOR:
		indent; printf("%s: {\n", flt_operatorid_names[expr->op.id]);
		print_exprnode(expr->op.left, depth + 1);
		print_exprnode(expr->op.right, depth + 1);
		indent; printf("}\n");
		break;

	case Flt_ET_NUMBERLITERAL:	indent; printf("%f\n", expr->num); break;
	case Flt_ET_STRINGLITERAL:	indent; printf("\"%s\"\n", expr->str); break;
	case Flt_ET_VARIABLE:		indent; printf("%s\n", expr->identifier); break;

	default:
		break;
	}
}

static void print_expression(const Flt_ExprNode* expr, const char* name, int depth)
{
	indent; printf("%s: {\n", name);
	print_exprnode(expr, depth + 1);
	indent; printf("}\n");
}



static void print_statementblock(const Flt_StatementBlock* block, const char* name, int depth);

static void print_statement(const Flt_StatementNode* stmt, int depth)
{
	indent; printf("%s: {\n", flt_statementnodetype_names[stmt->type]);
	switch (stmt->type)
	{
	case Flt_ST_EXPRESSION:
		print_expression(stmt->stmt_expr.expression, "expr", depth + 1);
		break;

	case Flt_ST_IF:
		print_expression(stmt->stmt_if.condition, "condition", depth + 1);
		print_statementblock(stmt->stmt_if.body_ontrue, "ontrue", depth + 1);
		print_statementblock(stmt->stmt_if.body_onfalse, "onfalse", depth + 1);
		break;

	default:
		break;
	}
	indent; printf("}\n");
}

static void print_statementblock(const Flt_StatementBlock* block, const char* name, int depth)
{
	if (!block) return;

	indent; printf("%s: {\n", name);
	Flt_StatementNode* iterator = block->statements.begin;
	while (iterator)
	{
		print_statement(iterator, depth + 1);
		iterator = iterator->next;
	}
	indent; printf("}\n");
}



void Flt_PrintCodeTree(const Flt_StatementBlock* code)
{
	printf("code: {\n");
	print_statementblock(code, "top", 1);
	printf("}\n");
}

