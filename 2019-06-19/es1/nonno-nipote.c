#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/prctl.h>

int main(int argc, char ** argv) {
	prctl(PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
	//creating a son process
	int sonPid=fork();
	if(sonPid==0){
		printf("[Son]: My pid: %d\n", getpid());
		//child process
		//create another child process
		int nephewPid=fork();
		if(nephewPid==0){
			printf("[Nephew]: My pid: %d\n", getpid());
			//nephew process ancoring to the ancestor
			prctl(PR_SET_CHILD_SUBREAPER, 1);
			//nephew process wait then exit
			sleep(10);
			exit(EXIT_SUCCESS);
		}
		//child process ends
		else exit(EXIT_SUCCESS);
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
