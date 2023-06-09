#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../../lib/list.h"

//delimitation string
#define DELIM "//"

/**
 * Splits the given string in substrings.
 * @param bigStr the string.
 * @param delim delimitator.
 * @return a list of substrings.
 */
List splitString(char *, char *);

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

int main(int argc, char ** argv) {
	//error
	if(argc<2) err(EXIT_FAILURE, "need more argv");
	//get the list of commands
	List cmdsList=cmdsFromArgv(argc-1, &argv[1]);
	//execute the list of commands
	para(cmdsList);
	exit(EXIT_SUCCESS);
}

List splitString(char * bigStr, char * delim) {
	char * safety;
	List listString=createList();
	char * tmp=malloc(strlen(bigStr)+1);
	tmp=strcpy(tmp, bigStr);
	char * token=strtok_r(tmp, delim, &safety);
	do {
		ListNode * node=createListNode();
		node->data=token;
		listString=addListNode(listString, node);
	} while((token=strtok_r(NULL, delim, &safety))!=NULL);
	return listString;
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
	return splitString(c, DELIM);
}

void para(List cmds) {
	//iterate on the commands
	List iterator=cmds;
	do {
		//create a son process to execute a command
		pid_t pId=fork();
		//son case
		if(pId==0) {
			List splittedCmd=splitString(iterator->data, " ");
			int len;
			char ** arr=(char **)listToArray(splittedCmd, &len);
			execvp(arr[0], arr);
			//returns only if error
			exit(EXIT_FAILURE);
		}
	} while((iterator=iterator->next)!=cmds);
	//wait for all the children to end
	int status;
	while(wait(&status)>0);
}
