#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <execs.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define PROCDIR "/proc/"
#define CMDLINE "/cmdline"
#define ENVIRON "/environ"

#define MAX_LEN 10000

typedef int fd_t;

int main(int argc, char ** argv) {
	//errors
	if(argc!=2) err(EXIT_FAILURE, "argc");
	//get the command and the environ
	char * cmdPath=malloc(sizeof(char)*(strlen(PROCDIR)+strlen(argv[1])+strlen(CMDLINE)+1));
	char * envPath=malloc(sizeof(char)*(strlen(PROCDIR)+strlen(argv[1])+strlen(ENVIRON)+1));
	cmdPath=strcpy(cmdPath, PROCDIR);
	cmdPath=strcat(cmdPath, argv[1]);
	cmdPath=strcat(cmdPath, CMDLINE);
	envPath=strcpy(envPath, PROCDIR);
	envPath=strcat(envPath, argv[1]);
	envPath=strcat(envPath, ENVIRON);
	fd_t fd;
	if((fd=open(cmdPath, O_RDONLY))==-1) err(EXIT_FAILURE, "open");
	char cmd[MAX_LEN];
	int byte=read(fd, cmd, MAX_LEN);
	close(fd);
	if((fd=open(envPath, O_RDONLY))==-1) err(EXIT_FAILURE, "open");
	char environ[MAX_LEN];
	byte=read(fd, environ, MAX_LEN);
	close(fd);
	execve(cmd, cmd, environ);
}
