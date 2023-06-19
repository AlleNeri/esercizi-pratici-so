#include <err.h>
#include <fcntl.h>
#include <execs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define MAX_BUFF (16*(EVENT_SIZE+16))

typedef int fd_t;

/**
* Join dir and file in a path.
* @param dir the directory path.
* @param file the file name.
* @return the file path.
*/
char * joinPath(char *, char *);

int main(int argc, char ** argv) {
	//error
	if(argc!=2) err(EXIT_FAILURE, "argv");
	//alias
	char * dir=argv[1];
	//monitorin the directory
	fd_t inotIstance=inotify_init();
	if(inotIstance<0) err(EXIT_FAILURE, "inotify_init");
	fd_t inotWatch=inotify_add_watch(inotIstance, dir, IN_CREATE);
	if(inotWatch<0) err(EXIT_FAILURE, "inotify_add_watch: can't open: %s", dir);
	//polling
	for(;;) {
		//waiting for event
		char buf[MAX_BUFF];
		int len=read(inotIstance, buf, MAX_BUFF);
		//handling event
		if(len==-1) err(EXIT_FAILURE, "read");
		const struct inotify_event * event;
		for(char * ptr=buf; ptr<buf+len; ptr+=EVENT_SIZE+event->len) {
			event=(const struct inotify_event *)ptr;
			if(!(event->mask & IN_ISDIR) && event->len) {
				char * path=joinPath(dir, event->name);
				printf("\t[DEBUG]: %s\n", path);
				fd_t fileFd=open(path, O_WRONLY, 0640);
				if(fileFd==-1) err(EXIT_FAILURE, "open");
				dup2(fileFd, STDOUT_FILENO);
				execsp(event->name);
			}
		}
	}
	exit(EXIT_FAILURE);
}

char * joinPath(char * dir, char * name) {
	char * res=malloc(sizeof(char)*(strlen(dir)+strlen(name)+2));
	res=strcpy(res, dir);
	if(dir[strlen(dir)-1]!='/') res=strcat(res, "/");
	res=strcat(res, name);
	return res;
}
