#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../../lib/bool.h"

typedef int fd_t;
typedef int pid_t;

/**
 * Parsing the string into signal and pid.
 * @param str the string to parse.
 * @param sig the signal to find out.
 * @param pId the pid to find out.
 * @return the execution status.
 */
bool parseSignal(char *, int *, pid_t *);

int main(int argc, char ** argv) {
	//error
	if(argc!=2) err(EXIT_FAILURE, "argv");
	//alias
	char * path=argv[1];
	//creating the named pipe
	mkfifo(path, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH);
	int fd=open(path, O_RDONLY);
	const int MAXLEN=100;
	char buf[MAXLEN];
	for(;;) {
		//read
		int byteread=read(fd, buf, MAXLEN-1);
		buf[byteread]='\0';
		if(strncmp(buf, "FINE\0", 5)==0) break;
		//print
		if(byteread>0) {
			//parsing the line
			int signal;
			pid_t pId;
			if(!parseSignal(buf, &signal, &pId)) {
				printf("Can't parse the line: %s\n", buf);
				continue;
			}
			//sending the signal
			printf("%s(=> sig: %d, pid: %d)\n", buf, signal, pId);
			kill(pId, signal);
		}
	}
	//closing the named pipe
	close(fd);
	remove(path);
	exit(EXIT_SUCCESS);
}

bool parseSignal(char * str, int * sig, pid_t * pId) {
	int successfullyMatch=sscanf(str, "%d %d", pId, sig);
	if(successfullyMatch!=2) return False;
	return True;
}
