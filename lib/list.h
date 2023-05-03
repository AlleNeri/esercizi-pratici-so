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
 * @param l the pointer to the List.
 * @param n the ListNode.
 * @return the data in the node(free it if necessary), NULL if something gose wrong.
 */
void * removeListNode(List *, ListNode *);

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
 * @param freeData true if is necessary to free the data, false otherwise.
 * @return the operation result.
 */
bool removeList(List *, bool);

/**
 * List length.
 * @param l the list.
 * @return the length.
 */
int listLen(List);

/**
 * List to array. ATTENTION: it will be an array of ptr to the same object of the list.
 * @param l the list.
 * @param len the array length.
 * @return the array.
 */
void ** listToArray(List, int *);
