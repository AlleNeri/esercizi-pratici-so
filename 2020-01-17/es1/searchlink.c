#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
	char * rPath;
	__ino_t inode;
	__mode_t mode;
} FileInfo;

/**
* Get the information of the file.
* @param f the file path.
* @return file info.
*/
FileInfo getFileInfo(char *);

/**
* Search links for a file in a directory.
* @param info the file info.
* @param dir the directory path.
*/
void searchLink(FileInfo, char *);

int main(int argc, char ** argv) {
	//error
	if(argc!=3) err(EXIT_FAILURE, "need more argv");
	//aliasing
	char * filePath=argv[1], * dirPath=argv[2];
	//get info of the file
	FileInfo fInfo=getFileInfo(filePath);
	searchLink(fInfo, dirPath);
	exit(EXIT_SUCCESS);
}

FileInfo getFileInfo(char * f) {
	FileInfo info;
	info.rPath=realpath(f, NULL);
	struct stat s;
	if(lstat(f, &s)==-1) err(EXIT_FAILURE, "lstat");
	info.mode=s.st_mode;
	info.inode=s.st_ino;
	return info;
}

void searchLink(FileInfo info, char * dir) {
	//open the dir
	DIR * d=opendir(dir);
	if(dir==NULL) err(EXIT_FAILURE, "opendir");
	//get all directory entry
	struct dirent * entry;
	while((entry=readdir(d))!=NULL) {
		//filter . and  ..
		if(strncmp(entry->d_name, ".", 2)==0 || strncmp(entry->d_name, "..", 3)==0) continue;
		int tmpPathLen=strlen(dir)+strlen(entry->d_name)+2;
		char * tmpPath=malloc(sizeof(char)*tmpPathLen);
		tmpPath=strncpy(tmpPath, dir, tmpPathLen);
		tmpPath=strncat(tmpPath, "/", tmpPathLen);
		tmpPath=strncat(tmpPath, entry->d_name, tmpPathLen);
		FileInfo i=getFileInfo(tmpPath);
		if(S_ISDIR(i.mode)) searchLink(info, tmpPath);
		else if (S_ISLNK(i.mode) && strcmp(info.rPath, i.rPath)==0) printf("symlink %s\n", tmpPath);
		else if (info.inode==i.inode) printf("link %s\n", tmpPath);
	}
}
