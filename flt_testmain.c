#include <stdio.h>

#include "fltu_string.h"
#include "flt_parse.h"
#include "flt_code.h"

int main(int argc, char** argv)
{
	Flt_List tokens = { 0 };
	Flt_StatementBlock* block = Flt_ParseSourceCode("if (5 < 10) { print(\"eyo\"); }");

	Flt_PrintCodeTree(block);

	return 0;
}