//eprova pratica 2019-05-29, sercizio 1.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/eventfd.h>

//boolean values
#define True 1
#define False 0

//type aliasing for better documentation
typedef int buffer_t;
typedef int semaphore_t;
typedef uint64_t semval_t;

//creating the buffer and the semaphores
buffer_t buffer;
semaphore_t semaphoreEmpty, semaphoreFull;

/**
 * Initialization.
 */
void init(void);

/**
 * Producer code.
 * @param arg arguments.
 * @return obfuscated.
 */
void * producer_routine(void *);

/**
 * Consumer code.
 * @param arg arguments.
 * @return obfuscated.
 */
void * consumer_routine(void *);

int main(int argc, char ** argv) {
	init();
	pthread_t producer_p, consumer_p;
	//creating threads
	pthread_create(&producer_p, NULL, producer_routine, NULL);
	pthread_create(&consumer_p, NULL, consumer_routine, NULL);
	//running threads
	pthread_join(producer_p, NULL);
	pthread_join(consumer_p, NULL);
	exit(EXIT_SUCCESS);
}

void init(void) {
	//setting random seed
	srand(time(NULL));
	//set semaphores
	semaphoreFull=eventfd(0, EFD_SEMAPHORE);
	semaphoreEmpty=eventfd(1, EFD_SEMAPHORE);
	if(semaphoreFull==-1 || semaphoreEmpty==-1) exit(EXIT_FAILURE);
}

void * producer_routine(void * args) {
	semval_t semval;
	while(True) {
		read(semaphoreEmpty, &semval, sizeof(semval_t));
		buffer=rand()%100;
		printf("Ok to read\n");
		semval=1;
		sleep(rand()%5);
		write(semaphoreFull, &semval, sizeof(semval_t));
	}
}

void * consumer_routine(void * args) {
	semval_t semval;
	while(True) {
		read(semaphoreFull, &semval, sizeof(semval_t));
		printf("The nuber is: %d\n",buffer);
		printf("Ok to write\n");
		semval=1;
		sleep(rand()%5);
		write(semaphoreEmpty, &semval, sizeof(semval_t));
	}
}
