#include <stdio.h>

#include "fltu_string.h"
#include "flt_parse.h"
#include "flt_code.h"

int main(int argc, char** argv)
{
	Flt_List tokens = { 0 };
	Flt_StatementBody* block = Flt_ParseSourceCode("a = 3 + 3");

	Flt_PrintCodeTree(block);

	return 0;
}