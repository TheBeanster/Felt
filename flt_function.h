#pragma once

#include "fltu_stdinc.h"

#include "flt_code.h"



typedef struct
{
	FltT_StatementBlock* statements;
	int numargs;
	char** argnames;
} Flt_Function;
