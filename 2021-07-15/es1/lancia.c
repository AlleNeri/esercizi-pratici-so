/**
 * Prima di eseguire questo è necessario creare una libreria nella cartella
 * /public a partire dall codice di hw.c presente in questa cartella.
 * Per compilare il codice in hw.c : gcc --shared -o /public/hw.so ./hw.c
 * IMPORTANTE: c'è un errore nel codice fornito dal prof nella traccia d'esame.
 * Nel codice che è presente in questa stessa cartella l'errore è fixato.
 */

#include <err.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int (* main_t) (int, char **);

int main(int argc, char ** argv) {
	//argc error
	if(argc<2) err(EXIT_FAILURE, "more arguments needed");
	char * lib=argv[1];
	//create the file absolute path
	char * libPath=malloc(strlen(lib)+3);
	libPath=strcpy(libPath, "./");
	libPath=strcat(libPath, lib);
	//opening the lib
	void * handle=dlopen(libPath, RTLD_LAZY);
	//error
	if(!handle) err(EXIT_FAILURE, "dlopen handle");
	dlerror();
	//getting the lib
	main_t libMain=dlsym(handle, "main");
	//error
	char * error=dlerror();
	if(error!=NULL) err(EXIT_FAILURE, "dlsym: %s", error);
	//executing the lib
	int res=libMain(argc, argv);
	//closing the lib
	dlclose(handle);
	exit(EXIT_SUCCESS);
}
