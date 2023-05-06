#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "../../lib/list.h"

/**
 * Get all directory entry in a directory.
 * @param d the directory.
 * @return a list of file.
 */
List getAllDirEnt(char *);

int main(int argc, char ** argv) {
	//errors
	if(argc!=2) err(EXIT_FAILURE, "argv");
	//alias
	char * dir=argv[1];
	int dirLen=strlen(dir);
	//get all directory entr
	List dirEntry=getAllDirEnt(dir);
	//print the output
	List iterator=dirEntry;
	do {
		char * path=malloc(dirLen+strlen(iterator->data)+1);
		path=strncat(path, dir, dirLen);
		path=strncat(path, iterator->data, strlen(iterator->data));
		char * realPath=realpath(path, NULL);
		printf("%s\n", realPath);
		free(realPath);
	} while((iterator=iterator->next)!=dirEntry);
	exit(EXIT_SUCCESS);
}

List getAllDirEnt(char * d) {
	//open the dir
	DIR * dir=opendir(d);
	if(dir==NULL) err(EXIT_FAILURE, "opendir");
	//get all directory entry
	List l=createList();
	struct dirent * entry;
	while((entry=readdir(dir))!=NULL) {
		//filter . and  ..
		if(strncmp(entry->d_name, ".", 2)==0 || strncmp(entry->d_name, "..", 3)==0) continue;
		ListNode * n=createListNode();
		n->data=entry->d_name;
		l=addListNode(l, n);
	}
	//close the dir
	if(closedir(dir)!=0) err(EXIT_FAILURE, "closedir");
	return l;
}
