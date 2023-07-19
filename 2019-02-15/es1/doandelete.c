#include <err.h>
#include <stdio.h>
#include <execs.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define MAX_BUFF (16*(EVENT_SIZE+16))

typedef enum { false, true } bool;

typedef int fd_t;

/**
* Check if a directory is empty.
* @param dir the directory path.
* @return the result of the checking.
*/
bool isEmpty(char *);

/**
* Check if a path is a directory.
* @param path the path to check.
* @return the result of the checking.
*/
bool isDir(char *);

/**
* Check if a path is an executable.
* @param path the path to check.
* @return the result of the checking.
*/
bool isExecutable(char *);

int main(int argc, char ** argv) {
	if(argc != 2) err(EXIT_FAILURE, "argc");
	//aliasing
	char * dir=argv[1];
	//checking
	if(!isDir(dir) || !isEmpty(dir)) err(EXIT_FAILURE, "No an empty directory");
	//adding to inotify event
	fd_t fd=inotify_init();
	if(fd == -1) err(EXIT_FAILURE, "inotify_init");
	int wd=inotify_add_watch(fd, dir, IN_MOVED_TO);
	if(wd<0) err(EXIT_FAILURE, "inotify_add_watch: can't open: %s", dir);
	//polling
	for(;;) {
		//waiting for event
		char buf[MAX_BUFF];
		int len=read(fd, buf, MAX_BUFF);
		//handling event
		if(len==-1) err(EXIT_FAILURE, "read");
		const struct inotify_event * event;
		for(char * ptr=buf; ptr<buf+len; ptr+=EVENT_SIZE+event->len) {
			event=(const struct inotify_event *)ptr;
			if(!(event->mask & IN_ISDIR) && event->len) {
				int filePathLen=event->len+strlen(dir)+1;
				char * filePath=malloc(sizeof(char)*filePathLen);
				filePath=strcpy(filePath, dir);
				filePath=strcat(filePath, "/");
				filePath=strncat(filePath, event->name, event->len);
				if(isExecutable(filePath)) {
					printf("\tExecuting %s\n", filePath);
					pid_t sonPid=fork();
					if(sonPid == 0) execsp(filePath);
					else wait(NULL);
				}
				remove(filePath);
				free(filePath);
			}
		}
	}
	exit(EXIT_FAILURE);
}

bool isEmpty(char * dir) {
	DIR * d = opendir(dir);
	if(d == NULL) err(EXIT_FAILURE, "opendir");
	struct dirent * ent;
	while((ent = readdir(d)) != NULL) {
		if(strcmp(ent->d_name, ".") == 0) continue;
		if(strcmp(ent->d_name, "..") == 0) continue;
		return false;
	}
	return true;
}

bool isDir(char * path) {
	struct stat s;
	if(stat(path, &s) == -1) err(EXIT_FAILURE, "stat");
	return S_ISDIR(s.st_mode);
}

bool isExecutable(char * path) {
	//reading the magic number
	FILE * f=fopen(path, "r");
	if(f == NULL) err(EXIT_FAILURE, "fopen");
	char magic[4];
	if(fread(magic, sizeof(char), 4, f) != 4) err(EXIT_FAILURE, "fread");
	fclose(f);
	//checking the magic number
	if(magic[0] != 0x7f) return false;
	if(magic[1] != 'E') return false;
	if(magic[2] != 'L') return false;
	if(magic[3] != 'F') return false;
	return true;
}
