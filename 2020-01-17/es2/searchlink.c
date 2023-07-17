#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

typedef short bool;
enum Bool {True=1, False=0};

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
* Search lnks for a file in a directory.
* @param info the file info.
* @param dir the directory path.
* @param copy copy the original file.
* @param lnk translate symlnk in lnk.
* @param symlnk translate lnk in symlnk.
*/
void searchLink(FileInfo, char *, bool, bool, bool);

/**
 * Create a copy of a file.
 * @param original the path of the original file.
 * @param copy the path of the copy.
 * @return the operation state.
 */
bool cpfile(char *, char *);

int main(int argc, char ** argv) {
	//error
	if(argc!=3) err(EXIT_FAILURE, "need more argv");
	//aliasing
	char * filePath=argv[1], * dirPath=argv[2];
	//get options
	char c;
	bool copy=False, lnk=False, symlnk=False;
	while((c=getopt(argc, argv, "cls:"))!=-1) {
		switch(c) {
			case 'c':
				copy=True;
				break;
			case 'l':
				lnk=True;
				break;
			case 's':
				symlnk=True;
				break;
			default: err(EXIT_FAILURE, "argv options");
		}
	}
	//get info of the file
	FileInfo fInfo=getFileInfo(filePath);
	searchLink(fInfo, dirPath, copy, lnk, symlnk);
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

void searchLink(FileInfo info, char * dir, bool copy, bool lnk, bool symlnk) {
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
		if(S_ISDIR(i.mode)) searchLink(info, tmpPath, copy, lnk, symlnk);
		else if (S_ISLNK(i.mode) && strcmp(info.rPath, i.rPath)==0) {
			printf("symlnk %s\n", tmpPath);
			if(copy || lnk) if(remove(tmpPath)==-1) err(EXIT_FAILURE, "remove");
			if(copy) if(!cpfile(info.rPath, tmpPath)) err(EXIT_FAILURE, "cpfile");
			if(lnk) if(link(info.rPath, tmpPath)==-1) err(EXIT_FAILURE, "link");
		}
		else if (info.inode==i.inode) {
			printf("lnk %s\n", tmpPath);
			if(copy || symlnk) if(remove(tmpPath)==-1) err(EXIT_FAILURE, "remove");
			if(copy) if(!cpfile(info.rPath, tmpPath)) err(EXIT_FAILURE, "cpfile");
			if(symlnk) if(symlink(info.rPath, tmpPath)==-1) err(EXIT_FAILURE, "symlink");
		}
	}
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
