#include "./list.h"
#include <stdio.h>

ListNode * createListNode() {
	ListNode * n=malloc(sizeof(ListNode));
	n->next=n->prev=NULL;
	n->data=NULL;
	return n;
}

List createList() {
	return NULL;
}

bool isEmptyList(List l) {
	if(l==NULL) return True;
	return False;
}

List addListNode(List l, ListNode * n) {
	//errors
	if(n->next!=NULL || n->prev!=NULL) return NULL;
	//empty list case
	if(isEmptyList(l)) {
		n->next=n->prev=n;
		return n;
	}
	//full list case
	n->next=l;
	n->prev=l->prev;
	l->prev=n;
	n->prev->next=n;
	return l;
}

bool removeListNode(List l, ListNode * n) {
	//errors
	if(n==NULL || n->next==NULL || n->prev==NULL) return False;
	if((n->next==n && n->prev!=n) || (n->next!=n && n->prev==n)) return False;
	//case of removing head
	if(l==n) l=n->next;
	//case with only one element
	if(n->next==n && n->prev==n) l=NULL;
	//full list case
	else {
		n->prev->next=n->next;
		n->next->prev=n->prev;
	}
	free(n->data);
	free(n);
	return True;
}

bool mergeList(List l1, List l2) {
	//errors
	if(isEmptyList(l1) || isEmptyList(l2)) return False;
	if(l1==l2) return True;
	//usage case
	l1->prev->next=l2;
	l2->prev->next=l1;
	ListNode * tmp=l2->prev;
	l2->prev=l1->prev;
	l1->prev=tmp;
	return True;
}

bool removeList(List l) {
	//delete all nodes
	while(!isEmptyList(l))
		//errors checking
		if(!removeListNode(l, l)) return False;
	return True;
}

int listLen(List l) {
	//empty list case
	if(isEmptyList(l)) return 0;
	//length counter
	int len=0;
	//counting
	List iterator=l;
	do len++;
	while((iterator=iterator->next)!=l);
	return len;
}

void ** listToArray(List l, int * len) {
	//length of the array
	*len=listLen(l);
	//allocate the array
	void ** arr=malloc(sizeof(void *)*(*len));
	//populate the array
	int i=0;
	List iterator=l;
	do arr[i++]=iterator->data;
	while((iterator=iterator->next)!=l);
	return arr;
}
