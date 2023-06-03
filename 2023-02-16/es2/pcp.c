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
 * @param n number of thread.
 */
bool filesCp(char *, char *, int);

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
	if(argc<=3) err(EXIT_FAILURE, "argc");
	//get j value
	int c, n=0;
	while((c=getopt(argc, argv, "j:"))!=-1) {
		if(c!='j') err(EXIT_FAILURE, "ergv j");
		n=atoi(optarg);
	}
	if(n<=0) err(EXIT_FAILURE, "argv j");
	//aliasing for the files
	char * file1=argv[3], * file2=argv[4];
	//copy the files
	if(!filesCp(file1, file2, n)) {
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

bool filesCp(char * original, char * new, int n) {
	//get the file dimension
	fdim_t originalFDim=fileDimension(original);
	//creating the new file
	fd_t fdTemp;
	if((fdTemp=creat(new, 0))==-1) err(EXIT_FAILURE, "creat");
	if(ftruncate(fdTemp, originalFDim)==-1) err(EXIT_FAILURE, "truncate");
	close(fdTemp);
	//creating sons
	pid_t pIds[n];
	off_t delta=(originalFDim-(originalFDim%n))/n, start=0;
	for(int i=0; i<n; i++) {
		//create a son process
		pIds[i]=fork();
		//son case
		if(pIds[i]==0) {
			off_t end=start+delta;
			if(i==n-1) end=originalFDim;
			//cp
			fCp(original, new, start, end);
			//returns only in case the file are equal
			exit(True);
		}
		start+=delta;
	}
	//parent case
	int status;
	while(wait(&status)>0) {
		if(status==EXIT_FAILURE) {
			//killing the other process
			for(int i=0; i<n; i++) kill(pIds[i], SIGKILL);
			return False;
		}
	}
	return status;
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
