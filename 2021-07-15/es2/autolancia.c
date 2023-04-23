/**
 * Prima di eseguire questo è necessario creare una libreria nella cartella
 * /public a partire dall codice di hw.c presente in questa cartella.
 * Per compilare il codice in hw.c : gcc --shared -o /public/hw.so ./hw.c && gcc -o /public/hw ./hw.c
 * IMPORTANTE: c'è un errore nel codice fornito dal prof nella traccia d'esame.
 * Nel codice che è presente in questa stessa cartella l'errore è fixato.
 */

#include <err.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>

//lib directory absolute path
#define libDir "/public/"

/**
 * Execute a shared lib.
 * @param path path to the shared lib.
 * @param argc argc for the lib.
 * @param argv argv for the lib.
 */
void execSharedLib(char *, int, char **);

/**
 * Try to execute the file in a son process.
 * @param path path to the file.
 * @param argv argv for the son.
 * @return -1 if the execution went wrong.
 */
int tryToExec(char *, char **);

int main(int argc, char ** argv) {
	//argc error
	if(argc<2) err(EXIT_FAILURE, "more arguments needed");
	char * file=argv[1];
	//create the file absolute path
	char * filePath=malloc(strlen(libDir)+strlen(file)+1);
	filePath=strcpy(filePath, libDir);
	filePath=strcat(filePath, file);
	//try to execute the file
	if(tryToExec(file, argv)) exit(EXIT_SUCCESS);
	//execute the shared lib
	else execSharedLib(file, argc, argv);
	exit(EXIT_SUCCESS);
}

void execSharedLib(char * lib, int argc, char ** argv) {
	//create the file absolute path
	char * libPath=malloc(strlen(libDir)+strlen(lib)+1);
	libPath=strcpy(libPath, libDir);
	libPath=strcat(libPath, lib);
	//opening the lib
	void * handle=dlopen(libPath, RTLD_LAZY);
	//error
	if(!handle) err(EXIT_FAILURE, "dlopen handle");
	dlerror();
	//getting the lib
	int (*libMain)(int,char **)=dlsym(handle, "main");
	//error
	char * error=dlerror();
	if(error!=NULL) err(EXIT_FAILURE, "dlsym: %s", error);
	//executing the lib
	int res=libMain(argc, argv);
}

int tryToExec(char * path, char ** argv) {
	//creating the son
	pid_t pId=fork();
	//son case
	if(pId==0) return execv(path, argv);
	//dad case
	else {
		int status=0;
		waitpid(pId, &status, 0);
		printf("status: %d, %d\n", status, WIFEXITED(status));
		return WIFEXITED(status);
	}
}
