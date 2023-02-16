#include "fltu_string.h"

#include "fltu_memory.h"



Flt_Bool Flt_SectionStringCompare(
	const char* parentstring,
	int			sectionstart,
	const char* compare
)
{
	// Extreme function!
	for (
		char* i = parentstring + sectionstart;	// i is the sectionstring iterator
		*compare != '\0';						// If reached the end of the compare string
		i++, compare++)							// Increment both pointers
	{
		if (*i == '\0' || *i != *compare) {
			return Flt_FALSE;
		}
	}
	return Flt_TRUE;
}

char* Flt_CopyCutString(const char* srcstring, int start, int length)
{
	char* str = Flt_MALLOC(length + 1); // Plus 1 to include null terminator

	for (int i = 0; i < length; i++)
	{
		str[i] = srcstring[i + start];
	}

	str[length] = '\0';
	return str;
}


