#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/prctl.h>

int main(int argc, char ** argv) {
	prctl(PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
	//set parent process name
	prctl(PR_SET_NAME, "Nonno", 0, 0, 0);
	//creating a son process
	int sonPid=fork();
	if(sonPid==0){
		//set son process name
		prctl(PR_SET_NAME, "Figlio", 0, 0, 0);
		printf("[Son]: My pid: %d\n", getpid());
		//child process
		//create another child process
		int nephewPid=fork();
		if(nephewPid==0){
			//set nephew process name
			prctl(PR_SET_NAME, "Nipote", 0, 0, 0);
			printf("[Nephew]: My pid: %d\n", getpid());
			//nephew process ancoring to the ancestor
			prctl(PR_SET_CHILD_SUBREAPER, 1);
			//nephew process wait then exit
			sleep(50);
			exit(EXIT_SUCCESS);
		}
		else {
			//child process waits and ends
			exit(EXIT_SUCCESS);
			sleep(30);
		}
	}
	else {
		//parent process wait for the nephew process
		int nephewPid=wait(NULL);	//son waiting
		nephewPid=wait(NULL);		//nephew waiting
		int p;
		prctl(PR_GET_CHILD_SUBREAPER, &p);
		printf("[Parent]: Nephew pid: %d\n", nephewPid);
	}
	exit(EXIT_SUCCESS);
}
