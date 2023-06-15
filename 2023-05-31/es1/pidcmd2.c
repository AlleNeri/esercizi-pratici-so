/** Soluzione revisionata **/
// Modifiche: utilizzo di memcmp
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define PROCDIR "/proc/"
#define CMDLINE "/cmdline"

typedef int fd_t;

/**
* Concatenate the vector of a command in a cmdline like string.
* @param argv the vector.
* @param len the lenght of the vector. Used as the lenght of the string in output.
* @return the srting cmd.
*/
char * getCmd(char **, int *);

int main(int argc, char ** argv) {
	//errors
	if(argc<=1) err(EXIT_FAILURE, "argc");
	//get the command
	int len=argc-1;
	char * command=getCmd(argv+1, &len);
	for(int i=0; i<len; i++) printf("%c", command[i]);
	printf("\nSearching for: %s\n", command);
	//opening the directory proc
	DIR * dirPtr;
	if((dirPtr=opendir(PROCDIR))==NULL) err(EXIT_FAILURE, "opendir");
	//read all directory entry
	struct dirent * dirEnt;
	while((dirEnt=readdir(dirPtr))!=NULL) {
		if(dirEnt->d_type!=DT_DIR) continue;
		//if it's a directory read it
		//get the path
		size_t dirEntNameLen=strnlen(dirEnt->d_name, 256);
		char * tmp=malloc(sizeof(char)*(strlen(PROCDIR)+dirEntNameLen+strlen(CMDLINE)+1));
		tmp=strcat(tmp, PROCDIR);
		tmp=strcat(tmp, dirEnt->d_name);
		tmp=strcat(tmp, CMDLINE);
		//open the file
		fd_t fd;
		if((fd=open(tmp, O_RDONLY))==-1) continue;
		//if the file exists, read it
		//read
		char * buf=malloc((len+1)*sizeof(char));
		int byteread=read(fd, buf, len+1);
		if(memcmp(command, buf, len)==0) printf("%s\n", dirEnt->d_name);
		close(fd);
	}
	closedir(dirPtr);
}

char * getCmd(char ** argv, int * len) {
	//get the lenght
	int c=0;
	for(int i=0; i<*len; i++) c+=strlen(argv[i]);//+1;
	//get the cmd
	char * cmd=malloc(sizeof(char)*c);
	char * tmp=cmd;
	for(int i=0; i<*len; i++) {
		int steps=strnlen(argv[i], c)+1;
		tmp=memcpy(tmp, argv[i], steps);
		tmp+=steps;
	}
	return cmd;
}
