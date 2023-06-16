#include <err.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/timerfd.h>

#define MAX_STR 1000

//conversions
#define s2ns(s) s*1000000000
#define ns2s(ns) ns/1000000000
#define s2ms(s) s*1000
#define ms2s(ms) ms/1000
#define ns2ms(ns) ns*1000000
#define ms2ns(ms) ms/1000000

typedef int fd_t;

/**
 * Timer status. First call initialize it.
 * @return the time elapsed
 */
struct timespec getTimer();

int main(int argc, char ** argv) {
	//error
	if(argc!=2) err(EXIT_FAILURE, "argv");
	//parsing the string
	int times;
	float tmpInterval;
	char * str=malloc(sizeof(char)*MAX_STR);
	sscanf(argv[1], "%d,%f,%s", &times, &tmpInterval, str);
	struct timespec interval;
	interval.tv_sec=tmpInterval;
	tmpInterval-=interval.tv_sec;
	interval.tv_nsec=s2ns(tmpInterval);
	//creating the timer
	fd_t timerFd=timerfd_create(CLOCK_REALTIME, 0);
	if(timerFd==-1) err(EXIT_FAILURE, "timerfd_create");
	//setting the timer
	struct itimerspec timerSetting;
	timerSetting.it_value=timerSetting.it_interval=interval;
	timerfd_settime(timerFd, 0, &timerSetting, NULL);
	//set timer reader
	getTimer();
	for(int i=0; i<times; i++) {
		//waiting for timer expiration
		char * buf=malloc(sizeof(char)*MAX_STR);
		if(read(timerFd, buf, sizeof(char)*MAX_STR)==-1) err(EXIT_FAILURE, "read");
		struct timespec actualTime=getTimer();
		double timeElaps=actualTime.tv_sec+ns2s((double)actualTime.tv_nsec);
		printf("%.6f %s\n", timeElaps, str);
	}
	//closing the timer
	close(timerFd);
}

struct timespec getTimer() {
	static int isFirstCall=1;
	static struct timespec startTime;
	struct timespec currTime={0, 0};
	if(isFirstCall) {
		isFirstCall=0;
		if(clock_gettime(CLOCK_MONOTONIC, &startTime)==-1) err(EXIT_FAILURE, "clock_gettime");
		if(clock_gettime(CLOCK_MONOTONIC, &currTime)==-1) err(EXIT_FAILURE, "clock_gettime");
	}
	if(clock_gettime(CLOCK_MONOTONIC, &currTime)==-1) err(EXIT_FAILURE, "clock_gettime");
	currTime.tv_sec-=startTime.tv_sec, currTime.tv_nsec-=startTime.tv_nsec;
	if(currTime.tv_nsec<0) {
		currTime.tv_sec--;
		currTime.tv_nsec+=s2ns(1);
	}
	return currTime;
}
