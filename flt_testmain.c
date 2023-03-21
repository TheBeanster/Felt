#include <stdio.h>

#include "fltu_string.h"
#include "flt_parse.h"
#include "flt_code.h"

int main(int argc, char** argv)
{
	printf("Testo\n%i\n", sizeof(Flt_Token));

	Flt_List tokens = { 0 };
	Flt_StatementBlock* block = Flt_ParseSourceCode("if 5 < 10 then");

	Flt_PrintCodeTree(block);

	return 0;
}