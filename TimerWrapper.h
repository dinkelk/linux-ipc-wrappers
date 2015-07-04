#include <time.h>
#include <errno.h>
#include <signal.h>

/*
// A wrapper around the posix timer system. It implements
// a downwards ticking one-shot timer that expires after the 
// specified time.
*/
typedef struct timer{
	timer_t timerid;
} timer;

int timerCreate(timer* timerid);
int timerDelete(timer* timerid);
int timerSet(timer* timerid, const time_t sec, const long nsec);
int timerSet_us(timer* timerid, const long usecs);
int timerUnset(timer* timerid);
int timerGet(timer* timerid, time_t* sec, long* nsec);
int isTimerExpired(timer* timerid);

/*
// A wrapper around the posix timer system. It implements
// a downwards ticking periodic timer that expires after the 
// specified time and calls a callback function.
*/
typedef struct periodicTimer{
	timer_t timerid;
	int signum;
	sigset_t set;
	int enabled;
} periodicTimer;

int periodicTimerCreate(periodicTimer* pt, long threadId, int signalNum);
int periodicTimerDelete(periodicTimer* pt);
int periodicTimerSet(periodicTimer* pt, const time_t sec, const long nsec);
int periodicTimerSet_us(periodicTimer* pt, const long usecs);
int periodicTimerUnset(periodicTimer* pt);
int periodicTimerGet(periodicTimer* pt, time_t* sec, long* nsec);
int periodicTimerWait(periodicTimer* pt, long sec, long nsec);
int periodicTimerIntervalGet(periodicTimer* pt, time_t* sec, long* nsec);
void periodicTimerEnable(periodicTimer* pt);
void periodicTimerDisable(periodicTimer* pt);
int isPeriodicTimerSet(periodicTimer* pt); 
int isPeriodicTimerEnabled(periodicTimer* pt);

/*
// An upwards counting stopwatch-like timer that can be used
// for timing system performance.
*/
typedef struct stopWatch{
	struct timespec start_t;
	struct timespec stop_t;
} stopWatch;

int stopWatchStart(stopWatch* sw);
int stopWatchStop(stopWatch* sw);
long stopWatchReport(stopWatch* sw);

/* The most accurate real-time sleeper: */
extern int rt_sleep(const time_t sec, const long nsec);
extern int rt_sleep_us(const long usec);

/* Most accurate way to get system time */
extern int rt_time( time_t* sec, long* nsec );
extern int rt_time_ts( struct timespec* systemTime );
unsigned long rt_time_ns();
unsigned long rt_time_nsts(struct timespec timeSpecStruct);

/* Utility functions: */
extern void usec_2_sec_nsec(long usec, time_t* sec, long* nsec);