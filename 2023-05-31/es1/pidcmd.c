/** Soluzione data al compito **/
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

char * getCmd(char **, int *);

char* replaceChar(char*, char, char);

int main(int argc, char ** argv) {
	//errors
	if(argc<=1) err(EXIT_FAILURE, "argc");
	//get the command
	int len=argc-1;
	char * command=getCmd(argv+1, &len);
	printf("Searching for: %s\n", command);
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
		int isEq=1;
		for(int i=0; i<len+1; i++) {
			if(buf[i]!=command[i]) isEq=0;
			if(isEq==0) break;
		}
		if(isEq==1) printf("%s\n", dirEnt->d_name);
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
	for(int i=0; i<*len; i++) {
		cmd=strncat(cmd, argv[i], c);
		cmd=strncat(cmd, " ", c);
	}
	*len=c;
	replaceChar(cmd, ' ', '\0');
	return cmd;
}

char* replaceChar(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}
