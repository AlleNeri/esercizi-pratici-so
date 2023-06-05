#include <err.h>
#include <stdio.h>
#include <execs.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SIZE 500

typedef int fd_t;

int main() {
	//reading the input
	char buffer[MAX_SIZE];
	//creating a common pipe for the two process
	fd_t pipeFd[2];
	if(pipe(pipeFd)==-1) err(EXIT_FAILURE, "pipe");
	while(fgets(buffer, MAX_SIZE, stdin)!=NULL) {
		//creating a son process who runs the command
		pid_t pId=fork();
		//son case
		if(pId==0) {
			//reading input from the pipe
			if(dup2(STDIN_FILENO, pipeFd[0])==-1) err(EXIT_FAILURE, "dup2");
			//catching the output in the pipe
			if(dup2(pipeFd[1], STDOUT_FILENO)==-1) err(EXIT_FAILURE, "dup2");
			close(pipeFd[0]);
			close(pipeFd[1]);
			//executing command
			execsp(buffer);
			//exit only in case of error
			exit(EXIT_FAILURE);
		}
		close(pipeFd[1]);
		//parent case
		int status;
		waitpid(pId, &status, 0);
		if(!WIFEXITED(status)) err(EXIT_FAILURE, "waitpid");
		//re-initializing common pipe
		int nBread;
		if((nBread=read(pipeFd[0], buffer, MAX_SIZE))==-1) err(EXIT_FAILURE, "read");
		close(pipeFd[0]);
		if(pipe(pipeFd)==-1) err(EXIT_FAILURE, "pipe");
		if(write(pipeFd[1], buffer, nBread)==-1) err(EXIT_FAILURE, "write");
	}
	//reading the output form the pipe
	read(pipeFd[0], buffer, MAX_SIZE);
	printf("%s", buffer);
	close(pipeFd[0]);
	close(pipeFd[1]);
	exit(EXIT_SUCCESS);
}
