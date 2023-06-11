#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ucontext.h>

/**
* Sigaction handler.
* @param signo signal number.
* @param info info.
* @param context the context.
*/
void handler(int, siginfo_t *, void *);

int main() {
	//printing the pId
	printf("%d\n", getpid());
	//preparing action
	struct sigaction action;
	action.sa_flags=SA_SIGINFO | SA_STACK;
	action.sa_sigaction=handler;
	sigemptyset(&action.sa_mask);
	if(sigaction(SIGUSR1, &action, NULL)==-1) err(EXIT_FAILURE, "sigaction");
	while(1) sleep(2);
	exit(EXIT_FAILURE);
}

void handler(int signo, siginfo_t * info, void * context) {
	//swapping stack to the sender stack to access his memory
	ucontext_t * senderContext=(ucontext_t *) context;
	stack_t * senderStack=&senderContext->uc_stack;
	if((senderStack->ss_sp=malloc(SIGSTKSZ))==NULL) err(EXIT_FAILURE, "malloc");
	senderStack->ss_size=SIGSTKSZ;
	senderStack->ss_flags=0;
	if(sigaltstack(senderStack, NULL)==-1) err(EXIT_FAILURE, "sigaltstack");
	//reading the messagge
	union sigval msg=info->si_value;
	printf("%p\n", msg.sival_ptr);
	printf("%s\n", (char *)msg.sival_ptr);
	exit(EXIT_SUCCESS);
}
