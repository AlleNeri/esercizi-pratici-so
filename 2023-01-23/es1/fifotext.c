#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef int fd_t;

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
	do {
		//read
		int byteread=read(fd, buf, MAXLEN-1);
		buf[byteread]='\0';
		//print
		if(byteread>0) printf("%s", buf);
	} while(strncmp(buf, "FINE\n", 5)!=0);
	//closing the named pipe
	close(fd);
	remove(path);
	exit(EXIT_SUCCESS);
}
