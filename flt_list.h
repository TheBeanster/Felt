#pragma once

#include "flt_stdinc.h"



typedef struct Flt_ListNode
{
	struct Flt_ListNode* prev;
	struct Flt_ListNode* next;
} Flt_ListNode;

typedef struct
{
	Flt_ListNode*	begin;
	Flt_ListNode*	end;
	int				count;
} Flt_List;

#define Flt_ListLinksHeader(type) struct type* prev; struct type* next
