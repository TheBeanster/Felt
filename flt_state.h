#pragma once

#include "fltu_stdinc.h"
#include "fltu_list.h"

#include "flt_code.h"



typedef struct
{
	Flt_List codebodies;

	char* errormsg;
} Flt_State;
