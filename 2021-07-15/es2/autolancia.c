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
 * Is executable or shared lib.
 * @param name the file name.
 * @return 0 if executable, 1 if shared lib.
 */
short exeOrShared(char *);

int main(int argc, char ** argv) {
	//argc error
	if(argc<2) err(EXIT_FAILURE, "more arguments needed");
	char * file=argv[1];
	exeOrShared(file);
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

short exeOrShared(char * name) {
	//how to recognize an executable and a shared lib?
}
