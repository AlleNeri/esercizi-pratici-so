#include <err.h>
#include <time.h>
#include <poll.h>
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
	if(argc<2) err(EXIT_FAILURE, "argv");
	//parsing the strings
	const int argCount=argc-1;
	int times[argCount];
	float tmpInterval[argCount];
	char * str[argCount];
	struct timespec interval[argCount];
	for(int i=0; i<argCount; i++) {
		str[i]=malloc(sizeof(char)*MAX_STR);
		sscanf(argv[i+1], "%d,%f,%s", &(times[i]), &(tmpInterval[i]), str[i]);
		interval[i].tv_sec=tmpInterval[i];
		tmpInterval[i]-=interval[i].tv_sec;
		interval[i].tv_nsec=s2ns(tmpInterval[i]);
	}
	//creating the timers
	struct pollfd fds[argCount];
	for(int i=0; i<argCount; i++) {
		fds[i].fd=timerfd_create(CLOCK_REALTIME, 0);
		if(fds[i].fd==-1) err(EXIT_FAILURE, "timerfd_create");
		fds[i].events=POLLIN;
	}
	//setting the timers
	for(int i=0; i<argCount; i++) {
		struct itimerspec timerSetting;
		timerSetting.it_value=timerSetting.it_interval=interval[i];
		if(timerfd_settime(fds[i].fd, 0, &timerSetting, NULL)==-1) err(EXIT_FAILURE, "timerfd_settime");
	}
	//set timer reader
	getTimer();
	//iterating
	int allTimes=0;
	for(int i=0; i<argCount; i++) allTimes+=times[i];
	for(int i=0; i<allTimes; ) {
		//setting the poll
		if(poll(fds, argCount, -1)==-1) err(EXIT_FAILURE, "poll");
		//get the timer expired
		for(int j=0; j<argCount; j++) {
			if(fds[j].revents==POLLIN && times[j]>0) {
				char * buf=malloc(sizeof(char)*MAX_STR);
				if(read(fds[j].fd, buf, sizeof(char)*MAX_STR)==-1) err(EXIT_FAILURE, "read");
				struct timespec actualTime=getTimer();
				double timeElaps=actualTime.tv_sec+ns2s((double)actualTime.tv_nsec);
				printf("%.6f %s\n", timeElaps, str[j]);
				times[j]--;
				i++;
			}
		}
	}
	//closing the timer
	for(int i=0; i<argCount; i++) close(fds[i].fd);
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
