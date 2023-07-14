#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define MAX_BUFF (16*(EVENT_SIZE+16))

typedef int fd_t;

/**
* Moves the content of the file f1 to the end of the file f2.
* @param f1 file src.
* @param f2 file dest.
*/
void moveContent(char *, char *);

int main(int argv, char ** argc) {
	//err
	if(argv!=3) err(EXIT_FAILURE, "argc");
	//aliasing
	char * dir=argc[1], * file=argc[2];
	//monitorin the directory
	fd_t inotIstance=inotify_init();
	if(inotIstance<0) err(EXIT_FAILURE, "inotify_init");
	fd_t inotWatch=inotify_add_watch(inotIstance, dir, IN_MOVED_TO);
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
				char * tmpFile=malloc(sizeof(char)*(strlen(event->name)+strlen(dir)+2));
				tmpFile=strcpy(tmpFile, dir);
				tmpFile=strcat(tmpFile, "/");
				tmpFile=strcat(tmpFile, event->name);
				moveContent(tmpFile, file);
				remove(tmpFile);
				free(tmpFile);
			}
		}
	}
	exit(EXIT_FAILURE);
}

void moveContent(char * f1, char * f2) {
	//read from the first file
	FILE * fStream1=fopen(f1, "r");
	if(fStream1==NULL) err(EXIT_FAILURE, "fopen");
	if(fseek(fStream1, 0L, SEEK_END)!=0) err(EXIT_FAILURE, "fseek");
	long f1Size=ftell(fStream1);
	rewind(fStream1);
	char * buffer=malloc(sizeof(char)*f1Size);
	long nItem=fread(buffer, sizeof(*buffer), f1Size, fStream1);
	fclose(fStream1);
	//print a header
	FILE * fStream2=fopen(f2, "a");
	if(fStream2==NULL) err(EXIT_FAILURE, "fopen");
	int headerLen=strlen(f1)+3;
	char * header=malloc(sizeof(char)*headerLen);
	header=strncpy(header, "\t", headerLen);
	header=strncat(header, f1, headerLen);
	header=strncat(header, "\n", headerLen);
	fwrite(header, sizeof(*header), headerLen, fStream2);
	free(header);
	//print the file content
	fwrite(buffer, sizeof(*buffer), f1Size, fStream2);
	free(buffer);
	fclose(fStream2);
}
