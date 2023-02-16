#include "fltu_string.h"

int Flt_SectionStringCompare(const char* parentstring, int sectionbegin, int sectionlen, const char* compare)
{
    const unsigned char *s1 = (const unsigned char *) parentstring;
    const unsigned char *s2 = (const unsigned char *) compare;
    unsigned char c1, c2;
    for (int i = 0; i < sectionlen; i++)
    {
        c1 = (unsigned char) *s1++;
        c2 = (unsigned char) *s2++;
        if (c2 == '\0')
            return c1 - c2;
    }
}
