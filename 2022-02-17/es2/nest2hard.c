#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
* Join dir and file in a path.
* @param dir the directory path.
* @param file the file name.
* @return the file path.
*/
char * joinPath(char *, char *);

/**
 * Get the file pointed by the link.
 * @param path the path to the link.
 * @return the file pointed.
 */
char * getLink2(char *);

/**
* Check if a symlink is nested.
* @param path the path to the symlink.
* @return the result of the check.
*/
int isNested(char *);

/**
* Move a symlink to an hard link.
* @param path to the symlink.
*/
void sym2hard(char *);

int main(int argc, char ** argv) {
	//error
	if(argc!=2) err(EXIT_FAILURE, "argv");
	//alias
	char * dirPath=argv[1];
	//iterating the directory
	DIR * dir=opendir(dirPath);
	if(dir==NULL) err(EXIT_FAILURE,  "opendir");
	struct dirent * iter;
	while((iter=readdir(dir))!=NULL) {
		//get file type
		if(iter->d_type!=DT_LNK) continue;
		char * actualFile=joinPath(dirPath, iter->d_name);
		//remove the nested link
		if(isNested(actualFile)) sym2hard(actualFile);
		free(actualFile);
	}
	closedir(dir);
}

char * joinPath(char * dir, char * name) {
	char * res=malloc(sizeof(char)*(strlen(dir)+strlen(name)+2));
	res=strcpy(res, dir);
	if(dir[strlen(dir)-1]!='/') res=strcat(res, "/");
	res=strcat(res, name);
	return res;
}

char * getLink2(char * path) {
	char * link2=malloc(sizeof(char)*PATH_MAX);
	if(readlink(path, link2, PATH_MAX)==-1) err(EXIT_FAILURE, "readlink");
	return link2;
}

int isNested(char * path) {
	char * link2=getLink2(path);
	struct stat fileStat;
	if(lstat(link2, &fileStat)==-1) err(EXIT_FAILURE, "lstat");
	free(link2);
	return S_ISLNK(fileStat.st_mode);
}

void sym2hard(char * path) {
	char * link2=getLink2(path);
	if(remove(path)==-1) err(EXIT_FAILURE, "remove");
	printf("Switching %s...\n", path);
	link(link2, path);
	free(link2);
}
