#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define PATH_MAX 4096

/**
* Join dir and file in a path.
* @param dir the directory path.
* @param file the file name.
* @return the file path.
*/
char * joinPath(char *, char *);

/**
 * Get the dir of a file.
 * @param f the file path.
 * @return the directory path.
 */
char * getDir(char *);

/**
 * Get the file name of a path.
 * @param f the file path.
 * @return the file name;
 */
char * getFileName(char *);

/**
 * Get the file pointed by the link.
 * @param path the path to the link.
 * @return the file pointed.
 */
char * getLink2(char *);

/**
* Switching symlink and file.
* @param path the path of the link.
*/
void switcSymlink(char *);

int main(int argc, char ** argv) {
	//error
	if(argc!=2) err(EXIT_FAILURE, "argv");
	//alias
	char * filePath=argv[1];
	//check if it's a symlink
	struct stat fileStat;
	if(lstat(filePath, &fileStat)==-1) err(EXIT_FAILURE, "lstat");
	if(!S_ISLNK(fileStat.st_mode)) exit(EXIT_SUCCESS);
	//it's a symlink
	switcSymlink(filePath);
	exit(EXIT_SUCCESS);
}

char * joinPath(char * dir, char * name) {
	char * res=malloc(sizeof(char)*(strlen(dir)+strlen(name)+2));
	res=strcpy(res, dir);
	if(dir[strlen(dir)-1]!='/') res=strcat(res, "/");
	res=strcat(res, name);
	return res;
}

char * getDir(char * f) {
	//lenght of the file name
	size_t len=0, fLen=strlen(f);
	for(unsigned int i=fLen-1; i>0; i--) {
		if(f[i]=='/') break;
		len++;
	}
	//get the directory path
	char * dirPath=malloc(fLen-len+1);
	dirPath=strncpy(dirPath, f, fLen-len);
	dirPath[fLen-len]='\0';
	return dirPath;
}

char * getFileName(char * f) {
	//lenght of the file name
	size_t len=0, fLen=strlen(f);
	for(unsigned int i=fLen-1; i>0; i--) {
		if(f[i]=='/') break;
		len++;
	}
	//get the file name
	char * fileName=malloc(len);
	fileName=strncpy(fileName, f+(fLen-len), len);
	fileName[len]='\0';
	return fileName;
}

char * getLink2(char * path) {
	char * link2=malloc(sizeof(char)*PATH_MAX);
	if(readlink(path, link2, PATH_MAX)==-1) err(EXIT_FAILURE, "readlink");
	return link2;
}

void switcSymlink(char * path) {
	char * link2=getLink2(path);
	//remove the link
	if(remove(path)==-1) err(EXIT_FAILURE, "remove");
	//move the file
	char * dir=getDir(path), * fileName=getFileName(link2);
	char * tmp=dir;
	dir=realpath(dir, NULL);
	if(dir==NULL) err(EXIT_FAILURE, "realpath");
	free(tmp);
	char * newFile=joinPath(dir, fileName);
	if(link(link2, newFile)==-1) err(EXIT_FAILURE, "link");
	if(remove(link2)==-1) err(EXIT_FAILURE, "remove");
	free(dir); free(fileName);
	//creating the new link
	dir=getDir(link2), fileName=getFileName(path);
	tmp=dir, dir=realpath(dir, NULL);
	if(dir==NULL) err(EXIT_FAILURE, "realpath");
	free(tmp);
	char * newLink=joinPath(dir, fileName);
	if(symlink(newFile, newLink)==-1) err(EXIT_FAILURE, "symlink");
	free(dir); free(fileName); free(newFile); free(newLink); free(link2);
}
