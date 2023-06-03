#include <err.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "../../lib/bool.h"

typedef off_t fdim_t;
typedef int fd_t;

/**
 * Open a file.
 * @param filePath the file path.
 * @return the file descriptor.
 */
fd_t openFile(char *);

/**
 * Get the file dimension in bytes.
 * @param f the file path.
 * @return the file dimension.
 */
fdim_t fileDimension(char *);

/**
 * Copy the files in parallel.
 * @param original the original file.
 * @param new the new file.
 */
bool filesCp(char * original, char * new);

/**
 * The sons file cp.
 * @param fOriginal original file path.
 * @param fNew new file path.
 * @param offToCopy oddset were start to copy.
 * @param offToCopy oddset were end to copy.
 */
void fCp(char *, char *, off_t, off_t);

int main(int argc, char ** argv) {
	//errors
	if(argc!=3) err(EXIT_FAILURE, "argc");
	//aliasing for the files
	char * file1=argv[1], * file2=argv[2];
	//copy the files
	if(!filesCp(file1, file2)) {
		printf("\t[DEBUG]: è finito in errore.\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

fd_t openFile(char * filePath) {
	//opening the file
	fd_t f=open(filePath, 0);
	if(f==-1) err(EXIT_FAILURE, "fopen");
	return f;
}

fdim_t fileDimension(char * f) {
	//get the files descriptor
	fd_t fd=openFile(f);
	fdim_t fileDim;
	//seek to the end of file returns the file dimension in bytes
	if((fileDim=lseek(fd, 0, SEEK_END))==-1) err(EXIT_FAILURE, "lseek");
	return fileDim;
}

bool filesCp(char * original, char * new) {
	//get the file dimension
	fdim_t originalFDim=fileDimension(original);
	//creating the new file
	fd_t fdTemp;
	if((fdTemp=creat(new, 0))==-1) err(EXIT_FAILURE, "creat");
	if(ftruncate(fdTemp, originalFDim)==-1) err(EXIT_FAILURE, "truncate");
	close(fdTemp);
	//create 2 sons
	pid_t pId1=fork();
	//1st son case
	if(pId1==0) {
		//cp
		fCp(original, new, 0, originalFDim/2);
		//ending sun process
		exit(True);
	}
	else {
		pid_t pId2=fork();
		//2nd son case
		if(pId2==0) {
			//cp
			fCp(original, new, originalFDim/2, originalFDim);
			//ending sun process
			exit(True);
		}
		//parent case
		else {
			int status;
			pid_t pIdEndded=wait(&status);
			if(status==False) {
				if(pIdEndded==pId1) kill(pId2, SIGKILL);
				if(pIdEndded==pId2) kill(pId1, SIGKILL);
				return False;
			}
			pIdEndded=wait(&status);
			return status;
		}
	}
}

void fCp(char * fOriginal, char * fCopy, off_t from, off_t to) {
	//opening the files
	FILE * fo, * fc;
	if((fo=fopen(fOriginal, "r"))==NULL || (fc=fopen(fCopy, "w"))==NULL) err(EXIT_FAILURE, "fopen");
	//move to the correct position
	if(fseek(fo, from*sizeof(char), SEEK_SET)==-1 || fseek(fc, from*sizeof(char), SEEK_SET)==-1) err(EXIT_FAILURE, "fseek");
	//offset to copy
	off_t offToCopy=to-from;
	//reading the offset
	size_t dim=sizeof(char)*offToCopy;
	char * buffer=malloc(dim);
	fread(buffer, sizeof(char), offToCopy, fo);
	//printing the the new file
	fwrite(buffer, sizeof(char), offToCopy, fc);
}
