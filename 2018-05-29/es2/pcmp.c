#include <err.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
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
 * Get the file dimension struct.
 * @param f the file path.
 * @return the file dimension.
 */
fdim_t fileDimension(char *);

/**
 * Compare the file in parallel.
 * @param fd1 file descriptor 1.
 * @param fd2 file descriptor 2.
 * @param n number of parallel process.
 * @return the result.
 */
bool filesCmp(char *, char *, int);

/**
 * The sons file comparator.
 * @param file1 file path 1.
 * @param file2 file path 2.
 * @param offToRead offset to be compared.
 */
void fCmp(fd_t, fd_t, off_t);

int main(int argc, char ** argv) {
	//errors
	if(argc!=5) err(EXIT_FAILURE, "argc");
	//get j value
	int c, n;
	while((c=getopt(argc, argv, "j:"))!=-1) {
		if(c!='j') err(EXIT_FAILURE, "ergv j");
		n=atoi(optarg);
	}
	if(n<=0) err(EXIT_FAILURE, "argv j");
	//aliasing for the files
	char * file1=argv[3], * file2=argv[4];
	//comparing the files
	if(filesCmp(file1, file2, n)==False) {
		printf("%s %s diff\n", file1, file2);
		exit(EXIT_SUCCESS);
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

bool filesCmp(char * file1, char * file2, int n) {
	//get dimensions
	fdim_t fdim1=fileDimension(file1);
	fdim_t fdim2=fileDimension(file2);
	//different dimensions
	if(fdim1!=fdim2) return False;
	pid_t pIds[n];
	off_t delta=fdim1/n, start=0;
	for(int i=0; i<n; i++) {
		//create a son process
		pIds[i]=fork();
		//son case
		if(pIds[i]==0) {
			//get different file descriptor
			fd_t fd1=openFile(file1), fd2=openFile(file2);
			//move the fd with a seek
			if(lseek(fd1, start, SEEK_SET)==-1 || lseek(fd2, start, SEEK_SET)==-1) err(EXIT_FAILURE, "lseek");
			//cmp
			fCmp(fd1, fd2, delta);
			//returns only in case the file are equal
			exit(True);
		}
		start+=delta;
	}
	//parent case
	int status;
	while(wait(&status)>0) {
		if(status==False) {
			//killing the other process
			for(int i=0; i<n; i++) kill(pIds[i], SIGKILL);
			return False;
		}
	}
	return status;
}

void fCmp(fd_t fd1, fd_t fd2, off_t offToRead) {
	//opening the file
	FILE * f1, * f2;
	if((f1=fdopen(fd1, "r"))==NULL || (f2=fdopen(fd2, "r"))==NULL) err(EXIT_FAILURE, "fdopen");
	//reading the offset
	size_t dim=sizeof(char)*offToRead;
	char * buffer1=malloc(dim), * buffer2=malloc(dim);
	fread(buffer1, sizeof(char), offToRead, f1);
	fread(buffer2, sizeof(char), offToRead, f2);
	//comparing char by char
	for(int i=0; i<dim; i++)
		if(buffer1[i]!=buffer2[i]) exit(False);
}
