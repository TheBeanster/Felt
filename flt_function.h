#pragma once

#include "fltu_stdinc.h"



typedef struct
{
	struct FltT_StatementBlock* statements;
	int numargs;
	char** argnames;
} Flt_Function;
