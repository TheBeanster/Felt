#pragma once

#include "fltu_stdinc.h"



typedef struct
{
	struct Flt_StatementBody* statements;
	int numargs;
	char** argnames;
} Flt_Function;



void Flt_DestroyFunction(Flt_Function* func);
