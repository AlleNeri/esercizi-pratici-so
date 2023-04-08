//In realtà non compila, ma penso sia un errore della libreria
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <execs.h>

//boolean values
#define True 1
#define False 0

//buffer lenght
#define BUFLEN 1024


int main(int argc, char ** argv) {
	//logout string
	char logout[]="logout";
	//command
	char * cmd=malloc(BUFLEN);
	//loop
	while(True) {
		cmd=NULL;
		printf(">");
		fgets(cmd, BUFLEN, stdin);
		if(strcmp(cmd, logout)==0) break;
		if(cmd==NULL || *cmd==(char)EOF) {
			printf("\n");
			break;
		}
		if(strlen(cmd)>=BUFLEN) {
			printf("Command to long.\n");
			continue;
		}
		execsp(cmd);
	}
	exit(EXIT_SUCCESS);
}
