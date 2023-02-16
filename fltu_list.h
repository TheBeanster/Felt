#pragma once

#include "fltu_stdinc.h"



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



/// @brief Pushes an element onto the beginning of a list.
/// @param list The list to push the element onto.
/// @param node Pointer to the element to push. Type must have the list links header!
void Flt_PushFrontList(
	Flt_List* const		list,
	Flt_ListNode* const	node
);

/// @brief Pushes an element onto the end of a list.
/// @param list The list to push the element onto.
/// @param node Pointer to the element to push. Type must have list links header!
void Flt_PushBackList(
	Flt_List* const		list,
	Flt_ListNode* const	node
);

/// @brief Removes the element at the beginning of the list and returns a pointer to it.
/// @param list The list to get the element from.
/// @return A pointer to the element or NULL if the list was empty. Make sure to know
/// the type of the element returned to cast it from the void* returned.
void* Flt_PopFrontList(
	Flt_List* const		list
);

/// @brief Removes the element at the end of the list and returns a pointer to it.
/// @param list The list to get the element from.
/// @return A pointer to the element or NULL if the list was empty. Make sure to know
/// the type of the element returned to cast it from the void* returned.
void* Flt_PopBackList(
	Flt_List* const		list
);

/// @brief Removes a node from a list and links the surrounding elements together.
/// @param list The list to unlink from.
/// @param node The node to unlink from the list.
void Flt_UnlinkFromList(
	Flt_List* const		list,
	Flt_ListNode* const	node
);

/// @brief Removes all elements from a list and calling the destroy_func on every element.
/// This does not free the list itself, only empties it.
/// @param list Pointer to the list to empty.
/// @param destroy_func Pointer to a destructor function, or NULL to use Flt_FREE().
void Flt_ClearList(
	Flt_List* const list,
	void(*destroy_func)(void*)
);
