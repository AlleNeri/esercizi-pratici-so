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

void * removeListNode(List * l, ListNode * n) {
	//errors
	if(n==NULL || n->next==NULL || n->prev==NULL) return NULL;
	if((n->next==n && n->prev!=n) || (n->next!=n && n->prev==n)) return NULL;
	//case of removing head
	if((*l)==n) (*l)=n->next;
	//case with only one element
	if(n->next==n && n->prev==n) l=NULL;
	//full list case
	else {
		n->prev->next=n->next;
		n->next->prev=n->prev;
	}
	void * ret=n->data;
	free(n);
	return ret;
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

bool removeList(List * l, bool freeData) {
	//delete all nodes
	int i=0;
	/*
	List iter=*l;
	do printf("%x\t", iter);
	while((iter=iter->next)!=*l);
	printf("\n");
	*/
	while(!isEmptyList(*l)) {
		//errors checking
		//printf("\t[DEBUG] Removing: n=%x={ prev: %x, next: %x }\n", (*l), (*l)->prev, (*l)->next);
		void * data=removeListNode(l, (*l));
		if(freeData) free(data);
		//printf("\t[DEBUG] Removed. Next: %x\n\tIteration: %d\n", (*l), ++i);
	}
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
