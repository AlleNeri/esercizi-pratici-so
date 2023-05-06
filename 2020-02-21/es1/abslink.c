#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * Get the dir of a file.
 * @param f the file path.
 * @return the directory path.
 */
char * getDir(char *);

int main(int argc, char ** argv) {
	//errors
	if(argc<2) err(EXIT_FAILURE, "argv");
	//alising
	char * linkPath=argv[1];
	//get the real path
	char * realPath=realpath(linkPath, NULL);
	//create the new link
	char * dirPath=getDir(realPath);
	size_t dirPathLen=strnlen(dirPath, strlen(realPath));
	char * newName="newFile.sym";
	size_t newNameLen=12;
	char * newPath=malloc(dirPathLen+strlen(newName)+1);
	newPath=strncat(newPath, dirPath, dirPathLen);
	newPath=strncat(newPath, newName, newNameLen);
	int status=symlink(realPath, newPath);
	//remove old file
	if(remove(linkPath)!=0) err(EXIT_FAILURE, "remove file");
	exit(EXIT_SUCCESS);
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
