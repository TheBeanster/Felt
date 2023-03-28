#include "flt_function.h"

#include "fltu_memory.h"
#include "flt_code.h"



void Flt_DestroyFunction(Flt_Function* func)
{
	if (func->argnames)
	{
		for (int i = 0; i < func->numargs; i++)
			Flt_FREE(func->argnames[i]);
		Flt_FREE(func->argnames);
	}
	Flt_DestroyStatementBlock(func->statements);
}
