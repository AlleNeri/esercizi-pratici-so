#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int argc, char ** argv) {
	//error
	if(argc!=3) err(EXIT_FAILURE, "argv");
	//aliasing
	pid_t pId=atoi(argv[1]);
	char * msg=argv[2];
	//test the pId and the msg
	union sigval sigVal;
	sigVal.sival_ptr=NULL;
	if(sigqueue(pId, 0, sigVal)==-1) err(EXIT_FAILURE, "Invalid pid");
	if(strnlen(msg, 10)>8) err(EXIT_FAILURE, "The messagge is to big");
	//sending the messagge
	sigVal.sival_ptr=msg;
	printf("%p\n", sigVal.sival_ptr);
	if(sigqueue(pId, SIGUSR1, sigVal)==-1) err(EXIT_FAILURE, "sigqueue");
	exit(EXIT_SUCCESS);
}
