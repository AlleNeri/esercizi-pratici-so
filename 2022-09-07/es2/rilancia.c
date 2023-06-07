#include <err.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * Fork and execute a command.
 * @param cmd the command vectror.
 * @return return status.
 */
int forkNexecv(char **);

int main(int argc, char ** argv) {
	//errors
	if(argc<2) err(EXIT_FAILURE, "argv");
	//aliasing
	char ** command=argv+1;
	//get start time
	time_t startTime=time(&startTime);
	//executing the command
	while(forkNexecv(command)!=-1) {
		//time check
		time_t currentTime=time(&startTime);
		if(startTime+1>=currentTime) exit(EXIT_SUCCESS);
		startTime=time(&startTime);
	}
	exit(EXIT_SUCCESS);
}

int forkNexecv(char ** cmd) {
	//creating the son process
	pid_t pId=fork();
	//son case: execute the command
	if(pId==0) {
		execv(cmd[0], cmd);
		//returns only in case of errors
		exit(EXIT_FAILURE);
	}
	//parent case: wait for son
	else if(pId>0) {
		int status;
		waitpid(pId, &status, 0);
		return WEXITSTATUS(status);
	}
	//error case
	return -1;
}
