#include "fltu_list.h"

#include "fltu_memory.h"



void Flt_PushFrontList(
	Flt_List* const		list,
	Flt_ListNode* const	node
)
{
	if (list->begin == NULL)
	{
		// List is empty
		node->prev = NULL;
		node->next = NULL;
		list->begin = node;
		list->end = node;
	} else
	{
		node->prev = NULL;
		node->next = list->begin;
		list->begin->prev = node;
		list->begin = node;
	}
	list->count++;
}



void Flt_PushBackList(
	Flt_List* const		list,
	Flt_ListNode* const	node
)
{
	if (list->begin == NULL)
	{
		// List is empty
		node->prev = NULL;
		node->next = NULL;
		list->begin = node;
		list->end = node;
	} else
	{
		node->prev = list->end;
		node->next = NULL;
		list->end->next = node;
		list->end = node;
	}
	list->count++;
}



void* Flt_PopFrontList(Flt_List* const list)
{
	Flt_ListNode* node = list->begin;
	Flt_UnlinkFromList(list, node);
	return node;
}



void* Flt_PopBackList(Flt_List* const list)
{
	Flt_ListNode* node = list->end;
	Flt_UnlinkFromList(list, node);
	return node;
}



void Flt_UnlinkFromList(
	Flt_List* const list,
	Flt_ListNode* const node
)
{
	if (node->prev)
		node->prev->next = node->next;
	else
		list->begin = node->next;

	if (node->next)
		node->next->prev = node->prev;
	else
		list->end = node->prev;

	node->prev = NULL;
	node->next = NULL;

	list->count--;
}



void Flt_ClearList(
	Flt_List* const list,
	void(*destroy_func)(void*)
)
{
	if (!list) return;
	if (list->count <= 0) return;
	if (list->begin == NULL) return;
	if (list->end == NULL) return;

	Flt_ListNode* iterator = list->begin;
	while (iterator)
	{
		Flt_ListNode* delblock = iterator;
		iterator = iterator->next;
		if (destroy_func)
			destroy_func(delblock);
		else
			Flt_FREE(delblock);
	}
	list->begin = NULL;
	list->end = NULL;
	list->count = 0;
}
