#include "flt_code.h"

#include "fltu_memory.h"



const char* flt_statementnodetype_names[Flt_NUM_STATEMENTTYPES] =
{
	"ST_EXPRESSION",
	"ST_IF",
	"ST_FOR",
	"ST_WHILE",
	"ST_DOWHILE",
	"ST_SCOPE",
};



void Flt_DestroyExpression(Flt_ExprNode* expr)
{
	if (!expr) return;
	
	switch (expr->type)
	{
	case Flt_ET_CLOSURE:
		Flt_DestroyFunction(expr->closure);
		break;

	case Flt_ET_OPERATOR:
		Flt_DestroyExpression(expr->op.left);
		Flt_DestroyExpression(expr->op.right);
		break;

	default:
		break;
	}
	Flt_FREE(expr);
}

void Flt_DestroyStatement(Flt_Statement* stmt)
{
}

void Flt_DestroyStatementBody(Flt_StatementBody* block)
{
}
