#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "../../lib/list.h"

//delimitation string
#define DELIM "//"

/**
 * Get a list of commands from the argv.
 * @param argc argv mumber.
 * @param argv argv array.
 * @return commands list.
 */
List cmdsFromArgv(int, char **);

/**
 * Execute in parallel a list of commands.
 * @param cmds commands list.
 */
void para(List);

/**
 * Execute the given command.
 * @param command the command.
 */
void * execute(void *);

int main(int argc, char ** argv) {
	//error
	if(argc<2) err(EXIT_FAILURE, "need more argv");
	//get the list of commands
	List cmdsList=cmdsFromArgv(argc-1, &argv[1]);
	//execute the list of commands
	para(cmdsList);
	removeList(cmdsList);
	exit(EXIT_SUCCESS);
}

List cmdsFromArgv(int argc, char ** argv) {
	//create a single string commands
	long fullLen=0;
	for(int i=0; i<argc; i++) fullLen+=1+strlen(argv[i]);
	char * c=malloc(fullLen+1);
	for(int i=0; i<argc; i++) {
		c=strcat(c, argv[i]);
		c=strcat(c, " ");
	}
	//get the list of single commands
	char * safety;
	List listCmds=createList();
	char * token=strtok_r(c, DELIM, &safety);
	do {
		ListNode * node=createListNode();
		node->data=token;
		addListNode(listCmds, node);
	} while((token=strtok_r(NULL, DELIM, &safety))!=NULL);
	return listCmds;
}

void para(List cmds) {
	//iterate on the commands
	List iterator=cmds;
	do {
		//create a thread for the process
		pthread_t t;
		pthread_create(&t, NULL, execute, iterator->data);
		printf("[DEBUG]: ora pro nobis\n");
		//execute the thread
		pthread_join(t, NULL);
	} while((iterator=iterator->next)!=cmds);
}

void * execute(void * command) {
	char ** tmp=NULL;
	execv(command, tmp);
	err(EXIT_FAILURE, "execv");
}
