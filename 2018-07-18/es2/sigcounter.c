#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/signalfd.h>

#include "./shared.h"

typedef int fd_t;

int globalCounter=0;

/**
 * Signal handler for SIGUSR1.
 */
void SIGUSR1_handler();

/**
 * Signal handler for SIGUSR2.
 */
void SIGUSR2_handler();

int main() {
	//musk the signal so they aren't handled by the default handler
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	sigaddset(&mask, SIGUSR2);
	if(sigprocmask(SIG_BLOCK, &mask, NULL)==-1) err(EXIT_FAILURE, "sigprocmask");
	//writing the pid on the file
	pid_t pId=getpid();
	FILE * fptr=fopen(MYFILE, "w");
	fprintf(fptr, "%d", pId);
	fclose(fptr);
	//signal fd
	fd_t sfd=signalfd(-1, &mask, 0);
	if(sfd==-1) err(EXIT_FAILURE, "signalfd");
	printf("Send to signals to pid: %d\nSIGUSR1: %d - SIGUSR2: %d\n", pId, SIGUSR1, SIGUSR2);
	printf("To send a signal use: kill -[signal value] [pid]\n");
	printf("Counter: %d\n", globalCounter);
	//capire come mandare i segnali al fd e poi finire di copiare l'esempio in `man signalfd` ;P
	struct signalfd_siginfo sfdi;
	//loop
	for(;;) {
		//reading the fd to catch the signals
		ssize_t s=read(sfd, &sfdi, sizeof(sfdi));
		if(s!=sizeof(sfdi)) err(EXIT_FAILURE, "read the signalfd_siginfo");
		//set the handleds
		if(sfdi.ssi_signo==SIGUSR1) SIGUSR1_handler();
		else if(sfdi.ssi_signo==SIGUSR2) SIGUSR2_handler();
		else if(sfdi.ssi_signo==SIGINT) exit(EXIT_SUCCESS);
		else printf("Invalid signal detected\n");
	}
}

void SIGUSR1_handler() {
	globalCounter++;
	printf("\tSIGUSR1 detected\nGlobal counter: %d\n", globalCounter);
}

void SIGUSR2_handler() {
	globalCounter--;
	printf("\tSIGUSR2 detected\nGlobal counter: %d\n", globalCounter);
}
