#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "../../lib/bool.h"

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
 * Switch a link to relative to absolute.
 * @param link the link.
 * @param baseDir the directory where create the new link.
 * @return the operation state.
 */
bool switchSymLink(char *, char *);

/**
 * Create a copy of a file.
 * @param original the path of the original file.
 * @param copy the path of the copy.
 * @return the operation state.
 */
bool cpfile(char *, char *);

int main(int argc, char ** argv) {
	//errors
	if(argc!=2) err(EXIT_FAILURE, "argv");
	//alising
	char * dir=argv[1];
	//opening the directory
	DIR * dirPtr;
	if((dirPtr=opendir(dir))==NULL) err(EXIT_FAILURE, "opendir");
	//read all directory entry
	struct dirent * dirEnt;
	while((dirEnt=readdir(dirPtr))!=NULL) {
		if(dirEnt->d_type!=DT_LNK) continue;
		//if it's a symlink create the absolute one
		size_t dirEntNameLen=strnlen(dirEnt->d_name, 256);
		char * tmp=malloc(sizeof(char)*(strlen(dir)+dirEntNameLen+1));
		tmp=strcat(tmp, dir);
		if(!switchSymLink(strncpy(tmp, dirEnt->d_name, dirEntNameLen), getDir(tmp))) err(EXIT_FAILURE, "symlink creation\n");
	}
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

bool switchSymLink(char * link, char * baseDir) {
	//get the real path
	char * realPath=realpath(link, NULL);
	//create the new link
	char * dirpath=getDir(realPath);
	size_t dirpathlen=strnlen(dirpath, strlen(realPath));
	char * newname=getFileName(realPath);
	size_t newnamelen=strnlen(newname, dirpathlen);
	char * newpath=malloc(dirpathlen+strlen(newname)+1);
	newpath=strncat(newpath, baseDir, strlen(baseDir)+1);
	newpath=strncat(newpath, newname, newnamelen);
	//remove old link
	if(remove(link)!=0) return False;
	//create the new link
	if(!cpfile(realPath, newpath)) return False;
	return True;
}

bool cpfile(char * original, char * copy) {
	FILE * ofptr, * cfptr;
	//open the orignial file
	if((ofptr=fopen(original, "r"))==NULL) return False;
	//open the copy file
	if((cfptr=fopen(copy, "w"))==NULL) return False;
	//read the file
	const size_t DIM;
	char r[DIM];
	while(fgets(r, DIM, ofptr)) fprintf(cfptr, "%s", r);
	return True;
}
