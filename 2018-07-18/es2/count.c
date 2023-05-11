#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "shared.h"

/**
 * Execute the given command.
 * @param len the array lenght.
 * @param cmd the array of the command.
 * @return the result.
 */
int execCmd(int, char **);

int main(int argc, char ** argv) {
	//check argv errors
	if(argc<=1) err(EXIT_FAILURE, "argc");
	//get the pid of the sigcount process
	FILE * fptr=fopen(MYFILE, "r");
	pid_t pId;
	fscanf(fptr, "%d", &pId);
	fclose(fptr);
	//increment the counter
	kill(pId, SIGUSR1);
	printf("\tsend 1\n");
	//execute the command
	int status=execCmd(argc-1, argv+1);
	//decrement the counter
	kill(pId, SIGUSR2);
	printf("\tsend 2\n");
	if(!status) err(EXIT_FAILURE, "exec status");
	exit(EXIT_SUCCESS);
}

int execCmd(int len, char ** cmd) {
	//create the child
	pid_t pId=fork();
	//son case
	if(pId==0) {
		printf("executing: ");
		for(int i=0; i<len; i++) printf("%s ", cmd[i]);
		printf("\n");
		//create NULL-ended array
		char ** cmdArray=malloc(sizeof(char *)*(len+1));
		for(int i=0; i<len; i++) cmdArray[i]=cmd[i];
		cmdArray[len]=NULL;
		execvp(cmd[0], cmd);
		//execv exit only in case of error
		err(EXIT_FAILURE, "execv\n");
	}
	//parent case
	else {
		int status;
		waitpid(pId, &status, 0);
		return WIFEXITED(status);
	}
}
