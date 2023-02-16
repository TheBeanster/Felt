#include <stdio.h>

#include "fltu_string.h"
#include "flt_parse.h"

int main(int argc, char** argv)
{
	printf("Testo\n%i\n", sizeof(Flt_Token));

	Flt_List tokens = { 0 };
	parse_sourcecode_tokens(&tokens, "function laf 0.0 (6 * 4)\n");

	for (Flt_Token* i = tokens.begin; i != NULL; i = i->next)
	{
		Flt_PrintToken(i);
		printf(" ", i->type);
	}

	return 0;
}