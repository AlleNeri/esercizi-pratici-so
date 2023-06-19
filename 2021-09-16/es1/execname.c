#include <err.h>
#include <execs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define MAX_BUFF (16*(EVENT_SIZE+16))

typedef int fd_t;

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
				remove(event->name);
				execsp(event->name);
			}
		}
	}
	exit(EXIT_FAILURE);
}
