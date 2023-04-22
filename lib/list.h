#define __LIST_H
#include <stdlib.h>
#ifndef __BOOL_H
	#include "./bool.h"
#endif

typedef struct ListNode ListNode;

struct ListNode {
	ListNode * prev, * next;
	void * data;
};

typedef ListNode * List;

/**
 * Create a list node.
 * @return list node.
 */
struct ListNode * createListNode();

/**
 * Create a list.
 * @return empty List.
 */
List createList();

/**
 * List is empty.
 * @param l the List.
 * @return true if is empty, false otherwise.
 */
bool isEmptyList(List);

/**
 * Adding a node to the list.
 * @param l the List.
 * @param n the ListNode.
 * @return the operation result.
 */
List addListNode(List, ListNode *);

/**
 * Removing the node from his list. It also delete the data memory location.
 * @param l the List.
 * @param n the ListNode.
 * @return the operation result.
 */
bool removeListNode(List, ListNode *);

/**
 * Merge two lists.
 * @param l1 first List.
 * @param l2 second List.
 * @return the operation result.
 */
bool mergeList(List, List);

/**
 * Removing the full list.
 * @param l the List.
 * @return the operation result.
 */
bool removeList(List);
