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
 * Get the file dimension struct.
 * @param f the file path.
 * @return the file dimension.
 */
fdim_t fileDimension(char *);

/**
 * Compare the file in parallel.
 * @param fd1 file descriptor 1.
 * @param fd2 file descriptor 2.
 * @return the result.
 */
bool filesCmp(char *, char *);

/**
 * The sons file comparator.
 * @param file1 file path 1.
 * @param file2 file path 2.
 * @param offToRead offset to be compared.
 */
void fCmp(fd_t, fd_t, off_t);

int main(int argc, char ** argv) {
	//errors
	if(argc!=3) err(EXIT_FAILURE, "argc");
	//aliasing for the files
	char * file1=argv[1], * file2=argv[2];
	//comparing the files
	if(filesCmp(file1, file2)==False) {
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

bool filesCmp(char * file1, char * file2) {
	//get dimensions
	fdim_t fdim1=fileDimension(file1);
	fdim_t fdim2=fileDimension(file2);
	//different dimensions
	if(fdim1!=fdim2) return False;
	//create 2 sons
	pid_t pId1=fork();
	//1st son case
	if(pId1==0) {
		//get differents file descriptor
		fd_t fd1c1=openFile(file1), fd2c1=openFile(file2);
		//cmp
		fCmp(fd1c1, fd2c1, fdim1/2);
		//returns only in case the file are equal
		exit(True);
	}
	else {
		pid_t pId2=fork();
		//2nd son case
		if(pId2==0) {
			//get differents file descriptor
			fd_t fd1c2=openFile(file1), fd2c2=openFile(file2);
			//move the fd with a seek
			if(lseek(fd1c2, fdim1/2, SEEK_SET)==-1 || lseek(fd2c2, fdim2/2, SEEK_SET)==-1) err(EXIT_FAILURE, "lseek");
			//cmp
			fCmp(fd1c2, fd2c2, fdim1/2-1);
			//returns only in case the file are equal
			exit(True);
		}
		//parent case
		else {
			int status;
			pid_t pIdEnded=wait(&status);
			if(status==False) {
				if(pIdEnded==pId1) kill(pId2, SIGKILL);
				if(pIdEnded==pId2) kill(pId1, SIGKILL);
				return False;
			}
			pIdEnded=wait(&status);
			return status;
		}
	}
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
