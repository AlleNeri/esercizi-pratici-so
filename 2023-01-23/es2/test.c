#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/signalfd.h>

typedef int pid_t;
typedef int fd_t;

int main(int argc, char ** argv) {
	//error
	if(argc!=2) err(EXIT_FAILURE, "argv");
	//setup for recv
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	if(sigprocmask(SIG_BLOCK, &mask, NULL)==-1) err(EXIT_FAILURE, "sigprocmask");
	//signal fd
	fd_t sfd=signalfd(-1, &mask, 0);
	if(sfd==-1) err(EXIT_FAILURE, "signalfd");
	struct signalfd_siginfo sfdi;
	//send pid and signal to the process
	pid_t pId=getpid();
	fd_t ffd=open(argv[1], O_WRONLY);
	char * line;
	sprintf(line, "%d %d", pId, SIGUSR1);
	if(write(ffd, line, sizeof(line))!=sizeof(line)) err(EXIT_FAILURE, "write");
	//get the signal
	ssize_t s=read(sfd, &sfdi, sizeof(sfdi));
	if(s!=sizeof(sfdi)) err(EXIT_FAILURE, "read the signalfd_siginfo");
	if(sfdi.ssi_signo==SIGUSR1) printf("Work fine\n");
	if(write(ffd, "FINE\0", 5)!=5) err(EXIT_FAILURE, "write");
	exit(EXIT_SUCCESS);
}
