#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

typedef int fd_t;

int indent;

/**
* Print the tree.
* @param path the path of the directory.
*/
void printTree(char *);

int main(int argc, char ** argv) {
	//error
	if(argc!=2) err(EXIT_FAILURE, "argv");
	//alias
	char * file=argv[1];
	//initialize indentation
	indent=0;
	printTree(file);
	exit(EXIT_SUCCESS);
}

void printTree(char * path) {
	//opening the directory
	fd_t dirFd=openat(0, realpath(path, NULL), 0);
	if(dirFd==-1) err(EXIT_FAILURE, "openat");
	DIR * dir=fdopendir(dirFd);
	//reading the directory entry
	struct dirent * ent;
	while((ent=readdir(dir))!=NULL) {
		if(strncmp(ent->d_name, ".", 2)==0 || strncmp(ent->d_name, "..", 3)==0) continue;
		printf("%.*s%s\n", indent, "\t\t\t\t\t\t\t\t\t\t\t", ent->d_name);
		if(ent->d_type==DT_DIR) {
			size_t dirEntNameLen=strnlen(ent->d_name, 256);
			char * tmp=malloc(sizeof(char)*(strlen(path)+dirEntNameLen+1));
			tmp=strcpy(tmp, path);
			tmp=strcat(tmp, "/");
			tmp=strcat(tmp, ent->d_name);
			indent++;
			printTree(tmp);
			indent--;
			free(tmp);
		}
	}
	closedir(dir);
	close(dirFd);
}
