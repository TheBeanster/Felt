#include "flt_code.h"



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
}

void Flt_DestroyStatement(Flt_StatementNode* stmt)
{
}

void Flt_DestroyStatementBlock(Flt_StatementBlock* block)
{
}
