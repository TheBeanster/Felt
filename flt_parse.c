#include "flt_parse.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fltu_memory.h"

#include "flt_code.h"



static Flt_Statement* parse_statement(const Flt_Token* tokenbegin, Flt_Token** tokenend);
static Flt_StatementBody* parse_statementbody(const Flt_Token* tokenbegin, Flt_Token** nexttoken);





static Flt_StatementBody* parse_singlestatement_body(
	const Flt_Token* tokenbegin,
	Flt_Token** tokenend
)
{
	Flt_Statement* bodytrue = parse_statement(tokenbegin, tokenend);
	if (!bodytrue)
	{
		printf("Couldn't parse single statement body\n");
		*tokenend = NULL;
		return NULL;
	}
	Flt_StatementBody* body = Flt_ALLOC_TYPE(Flt_StatementBody);
	Flt_PushBackList(&body->statements, bodytrue);
	return body;
}



/// @brief Parses a body between {...}
/// @param tokenbegin Pointer to token after '{'
/// @param nexttoken The token after closing '}'
/// @return The body in the scope
static Flt_StatementBody* parse_scope_statementbody(
	const Flt_Token* tokenbegin,
	Flt_Token** tokenend
)
{
	Flt_StatementBody* body = parse_statementbody(tokenbegin, &tokenbegin);
	if (!body || !tokenbegin || tokenbegin->separatorid != Flt_SP_RBRACE)
	{
		printf("Scope ended without '}'\n");
		Flt_DestroyStatementBody(body);
		*tokenend = NULL;
		return NULL;
	}
	*tokenend = tokenbegin->next;
	return body;
}



/// @brief Parses a body that may be a single statement, or more with {...}. Used for 'if', 'for' 'while' etc...
/// @param tokenbegin The token where the body begins
/// @param tokenend The token after the body ends
/// @return The body parsed
static Flt_StatementBody* parse_possiblyscoped_statementbody(
	const Flt_Token* tokenbegin,
	Flt_Token** tokenend
)
{
	// True condition body
	if (tokenbegin->separatorid != Flt_SP_LBRACE)
	{
		// Single statement body
		Flt_StatementBody* body = parse_singlestatement_body(tokenbegin, tokenend);
		return body;
	} else
	{
		// Scoped body
		tokenbegin = tokenbegin->next;
		if (!tokenbegin)
		{
			printf("Missing body after '{'\n"); *tokenend = NULL;  return NULL;
		}
		Flt_StatementBody* body = parse_statementbody(tokenbegin, &tokenbegin);
		if (!body || !tokenbegin || tokenbegin->separatorid != Flt_SP_RBRACE)
		{
			printf("Scope ended without '}'\n");
			Flt_DestroyStatementBody(body);
			return NULL;
		}
		*tokenend = tokenbegin->next;
		return body;
	}
}







typedef struct expr_listnode
{
	Flt_ListNode l_both_links;
	union
	{
		Flt_ListNode l_operator_links;
		Flt_ListNode l_operand_links;
	};
	Flt_ExprNode* node;
} expr_listnode;



static expr_listnode* create_boolkw_node(
	const Flt_Bool b
)
{
	expr_listnode* node = Flt_ALLOC_TYPE(expr_listnode);
	node->node = Flt_ALLOC_TYPE(Flt_ExprNode);
	node->node->type = Flt_ET_BOOLLITERAL;
	node->node->boolean = b;
	return node;
}

static expr_listnode* parse_numberliteral_node(
	const Flt_Token* token
)
{
	expr_listnode* node = Flt_ALLOC_TYPE(expr_listnode);
	node->node = Flt_ALLOC_TYPE(Flt_ExprNode);
	node->node->type = Flt_ET_NUMBERLITERAL;
	node->node->num = strtod(token->string, NULL);
	return node;
}

static expr_listnode* parse_stringliteral_node(
	const Flt_Token* token
)
{
	expr_listnode* node = Flt_ALLOC_TYPE(expr_listnode);
	node->node = Flt_ALLOC_TYPE(Flt_ExprNode);
	node->node->type = Flt_ET_STRINGLITERAL;
	node->node->str.chars = _strdup(token->string);
	node->node->str.len = strlen(token->string);
	return node;
}



static expr_listnode* parse_operator_node(
	const Flt_Token* token
)
{
	expr_listnode* node = Flt_ALLOC_TYPE(expr_listnode);
	node->node = Flt_ALLOC_TYPE(Flt_ExprNode);
	node->node->type = Flt_ET_OPERATOR;
	node->node->op.id = token->operatorid;
	return node;
}



/// @brief Parses an expression. This can be any expression such as if conditions or loop iterators
/// @param tokenbegin The token the expression begins at
/// @param tokenend Will be set to the last token found of the expression. '1 + 2;' will point to ';'. This could be null!
/// @return The top node of the expression tree generated
static Flt_ExprNode* parse_expression(
	const Flt_Token* tokenbegin,
	Flt_Token** tokenend
)
{
	printf("Parsing expression\n");

	Flt_ExprNode* expr = Flt_ALLOC_TYPE(Flt_ExprNode);

	Flt_List l_operators = { 0 }; // List of all operators
	Flt_List l_operands = { 0 }; // List of all operands
	Flt_List l_both = { 0 }; // List of all operands and operators
	
	Flt_Token* iterator = tokenbegin;
	while (iterator)
	{
		switch (iterator->type)
		{

		case Flt_TT_KEYWORD:
		{
			if (!((iterator->keywordid == Flt_KW_TRUE) || (iterator->keywordid == Flt_KW_FALSE)))
			{
				if (iterator->keywordid == Flt_KW_FUNCTION)
				{
					printf("Closure!!!\n");
				} else
					goto on_expr_end;
			}
			expr_listnode* n = create_boolkw_node(iterator->keywordid == Flt_KW_TRUE);
			Flt_PushBackList(&l_operands, &n->l_operand_links);
			Flt_PushBackList(&l_both, &n->l_both_links);
		}
			break;

		case Flt_TT_NUMBERLITERAL:
		{
			expr_listnode* n = parse_numberliteral_node(iterator);
			Flt_PushBackList(&l_operands, &n->l_operand_links);
			Flt_PushBackList(&l_both, &n->l_both_links);
		}
			break;

		case Flt_TT_STRINGLITERAL:
		{
			expr_listnode* n = parse_stringliteral_node(iterator);
			Flt_PushBackList(&l_operands, &n->l_operand_links);
			Flt_PushBackList(&l_both, &n->l_both_links);
		}
			break;

		case Flt_TT_SEPARATOR:
			if (iterator->separatorid == Flt_SP_LPAREN) // Subexpression
			{
				if (iterator->next == NULL)
				{
					printf("Code ends after '('\n");
					goto on_error;
				}

				expr_listnode* subexpr = Flt_ALLOC_TYPE(expr_listnode);
				subexpr->node = parse_expression(iterator->next, &iterator);
				if (!subexpr->node) goto on_error;
				if (iterator->separatorid != Flt_SP_RPAREN)
				{
					printf("Mismatching token '");
					Flt_PrintToken(iterator);
					printf("', expected ')'\n");
					goto on_error;
				}
				
				Flt_PushBackList(&l_operands, &subexpr->l_operand_links);
				Flt_PushBackList(&l_both, &subexpr->l_both_links);
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
		{
			expr_listnode* n = parse_operator_node(iterator);
			Flt_PushBackList(&l_operators, &n->l_operator_links);
			Flt_PushBackList(&l_both, &n->l_both_links);
		}
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

	*tokenend = iterator;

	printf("Expression operators = ");
	expr_listnode* iter = l_operators.begin;
	while (iter)
	{
		printf("   Op:%s   ", flt_operatorid_names[iter->node->op.id]);
		iter = iter->l_operator_links.next;
	}
	printf("\n");

	return expr;

on_error:
	Flt_FREE(expr);
	return NULL;
}




static Flt_Statement* parse_expression_statement(
	const Flt_Token* tokenbegin,
	Flt_Token** nextstatement
)
{
	Flt_Statement* stmt = Flt_ALLOC_TYPE(Flt_Statement);
	stmt->type = Flt_ST_EXPRESSION;
	stmt->stmt_expr.expression = parse_expression(tokenbegin, nextstatement);
	if (!stmt->stmt_expr.expression)
	{
		printf("Couldn't parse expression statement\n");
		Flt_FREE(stmt);
		return NULL;
	}
	if ((*nextstatement)->separatorid != Flt_SP_SEMICOLON) // Expression statement must end on semicolon
	{
		printf("Expression statement did not end in a vaild way\n");
		Flt_DestroyStatement(stmt);
		return NULL;
	}
	*nextstatement = (*nextstatement)->next;
	
	return stmt;
}




static Flt_Statement* parse_if_statement(
	const Flt_Token* tokenbegin,
	Flt_Token** tokenend
)
{
	// Make sure token after 'if' is ( and that there is a token after that too
	if (tokenbegin->next && tokenbegin->next->separatorid != Flt_SP_LPAREN && tokenbegin->next->next)
	{
		printf("Expected '(' after if keyword\n");
		return NULL;
	}

	// Parse the condition expression
	Flt_Token* token = tokenbegin->next->next;
	Flt_ExprNode* condition = parse_expression(token, &token);
	if (!condition || !token || token->separatorid != Flt_SP_RPAREN || !token->next) // If condition doesn't end with ')'
	{
		printf("Couldn't parse if statement condition expression\n");
		Flt_DestroyExpression(condition);
		*tokenend = NULL;
		return NULL;
	}

	Flt_Statement* stmt = Flt_ALLOC_TYPE(Flt_Statement);
	stmt->type = Flt_ST_IF;
	stmt->stmt_if.condition = condition;

	stmt->stmt_if.body_ontrue = parse_possiblyscoped_statementbody(token->next, &token);
	if (!stmt->stmt_if.body_ontrue) goto on_error;

	if (token && token->keywordid == Flt_KW_ELSE && token->next)
	{
		// There is an else body
		stmt->stmt_if.body_onfalse = parse_possiblyscoped_statementbody(token->next, &token);
		if (!stmt->stmt_if.body_onfalse) goto on_error;
	}

	*tokenend = token;
	return stmt;

on_error:
	// Free everything
	*tokenend = NULL;
	return NULL;
}



static Flt_Statement* parse_statement(
	const Flt_Token* tokenbegin,
	Flt_Token** tokenend
)
{
	printf(" > Parsing statement starting with '");
	Flt_PrintToken(tokenbegin);
	printf("'\n");

	Flt_Statement* statement = NULL;

	switch (tokenbegin->keywordid)
	{

	case Flt_KW_IF: 
		statement = parse_if_statement(tokenbegin, tokenend);
		if (!statement)
		{
			printf("Failed to parse if statement\n");
			return NULL;
		}
		return statement;



	default:
		// If statement doesn't start with a keyword then it is an expression, scope or invalid

		switch (tokenbegin->type)
		{
		case Flt_TT_IDENTIFIER:
		case Flt_TT_NUMBERLITERAL:
			statement = parse_expression_statement(tokenbegin, &tokenbegin);
			if (!statement)
			{
				printf("Failed to parse expression statement\n");
				return NULL;
			}
			*tokenend = tokenbegin;
			return statement;

		default:
			printf("Invalid statement start '");
			Flt_PrintToken(tokenbegin);
			printf("'\n");
			*tokenend = tokenbegin;
			return NULL;
		}
		
	}

	
}



static Flt_StatementBody* parse_statementbody(
	const Flt_Token* tokenbegin,
	Flt_Token** nexttoken
)
{
	Flt_StatementBody* body = Flt_ALLOC_TYPE(Flt_StatementBody);
	
	Flt_Token* iterator = tokenbegin;
	while (iterator)
	{
		Flt_Statement* node = parse_statement(iterator, &iterator);
		if (!node)
		{
			*nexttoken = iterator;
			return body;
		}
		Flt_PushBackList(&body->statements, node);
	}

	*nexttoken = NULL;
	return body;

on_fail:
	Flt_ClearList(&body->statements, &Flt_DestroyStatement);
	Flt_FREE(body);
	return NULL;
}







Flt_StatementBody* Flt_ParseSourceCode(const char* sourcecode)
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

	Flt_Token endtoken = { 0 };
	Flt_StatementBody* body = parse_statementbody(tokens.begin, &endtoken);
	Flt_PrintToken(&endtoken);

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



static void print_statementbody(const Flt_StatementBody* body, const char* name, int depth);

static void print_statement(const Flt_Statement* stmt, int depth)
{
	indent; printf("%s: {\n", flt_statementnodetype_names[stmt->type]);
	switch (stmt->type)
	{
	case Flt_ST_EXPRESSION:
		print_expression(stmt->stmt_expr.expression, "expr", depth + 1);
		break;

	case Flt_ST_IF:
		print_expression(stmt->stmt_if.condition, "condition", depth + 1);
		print_statementbody(stmt->stmt_if.body_ontrue, "ontrue", depth + 1);
		print_statementbody(stmt->stmt_if.body_onfalse, "onfalse", depth + 1);
		break;

	default:
		break;
	}
	indent; printf("}\n");
}

static void print_statementbody(const Flt_StatementBody* body, const char* name, int depth)
{
	if (!body) return;

	indent; printf("%s: {\n", name);
	Flt_Statement* iterator = body->statements.begin;
	while (iterator)
	{
		print_statement(iterator, depth + 1);
		iterator = iterator->next;
	}
	indent; printf("}\n");
}



void Flt_PrintCodeTree(const Flt_StatementBody* code)
{
	printf("code: {\n");
	print_statementbody(code, "top", 1);
	printf("}\n");
}

