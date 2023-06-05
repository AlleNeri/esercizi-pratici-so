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
	char line1[MAX_SIZE], line2[MAX_SIZE];
	fgets(line1, MAX_SIZE, stdin);
	fgets(line2, MAX_SIZE, stdin);
	//creating a common pipe for the two process
	fd_t pipeFd[2];
	if(pipe(pipeFd)==-1) err(EXIT_FAILURE, "pipe");
	//creating a son process who runs the first command
	pid_t pId=fork();
	//son case
	if(pId==0) {
		//catching the output in the pipe
		dup2(pipeFd[1], STDOUT_FILENO);
		close(pipeFd[0]);
		close(pipeFd[1]);
		//executing 1st command
		execsp(line1);
		//exit only in case of error
		exit(EXIT_FAILURE);
	}
	//parent case
	//send output of 1st commant as input of the 2nd command
	dup2(pipeFd[0], STDIN_FILENO);
	close(pipeFd[0]);
	close(pipeFd[1]);
	//executing 2nd command
	execsp(line2);
	//return only in case of error
	exit(EXIT_FAILURE);
}
