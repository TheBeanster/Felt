#pragma once

#include "fltu_stdinc.h"


// Compile memory management functions with raw standard library function calls
// or call Flt functions which have error checking.
// Compiling the normal standard library functions could be useful in some IDEs
// that keep track of allocations by checking calls to the standard functions.
#define Flt_COMPILE_USE_STD_ALLOCATION 1

#if Flt_COMPILE_USE_STD_ALLOCATION

#define Flt_MALLOC(size)			malloc(size)
#define Flt_CALLOC(count, size)		calloc(count, size)
#define Flt_REALLOC(block, size)	realloc(block, size)
#define	Flt_ALLOC_TYPE(type)		(type*)calloc(1, sizeof(type))
#define Flt_FREE(block)				free(block)

#else

void* Flt_Malloc(const size_t size);
void* Flt_Calloc(const int count, const size_t size);
void* Flt_Realloc(void* block, const size_t size);
void* Flt_Free(const void* const block);

#define Flt_MALLOC(size)			Flt_Malloc(size)
#define Flt_CALLOC(count, size)		Flt_Calloc(count, size)
#define	Flt_ALLOC_TYPE(type)		Flt_Calloc(1, sizeof(type))
#define Flt_FREE(block)				Flt_Free(block)

#endif