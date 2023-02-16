#pragma once

#include "fltu_stdinc.h"



Flt_Bool Flt_SectionStringCompare(
	const char* parentstring,
	int			sectionstart,
	const char* compare
);

char* Flt_CopyCutString(
	const char* srcstring,
	int start,
	int length
);
