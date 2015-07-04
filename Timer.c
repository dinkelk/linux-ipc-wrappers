#include "Timer.h"

int timerCreate(timer* timerid)
{
	/*
	// Create monotonic clock. It does not rely on the
	// system-wide real-time "settable" clock.
	// The monotonic clock is unsettable and is
	// good for relative timing...
	// Don't send signal on timer notify:
	*/
	struct sigevent sevp;
	sevp.sigev_notify = SIGEV_NONE;

	if( timer_create(CLOCK_MONOTONIC, &sevp, &timerid->timerid) < 0)
	{
		return -1;
	}

	return 0;
}

int timerDelete(timer* timerid)
{
	return timer_delete(timerid->timerid);
}

int timerSet(timer* timerid, const time_t sec, const long nsec)
{
	struct itimerspec timer;

	/* Disable interval timing, we want a "one-shot" timer: */
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = 0;

	/* Set timer to expire at desired relative time: */
	timer.it_value.tv_sec = sec;
	timer.it_value.tv_nsec = nsec;

	if( timer_settime(timerid->timerid, 0, &timer, NULL) < 0 )
	{
		return -1;
	}

	return 0;
}

int timerSet_us(timer* timerid, const long usec)
{
	struct itimerspec timer;

	/* Disable interval timing, we want a "one-shot" timer: */
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = 0;

	/* Set timer to expire at desired relative time: */
	timer.it_value.tv_sec = usec / 1000000;
	timer.it_value.tv_nsec = (usec % 1000000) * 1000;

	if( timer_settime(timerid->timerid, 0, &timer, NULL) < 0 )
	{
		return -1;
	}

	return 0;
}

int timerUnset(timer* timerid)
{
	struct itimerspec timer;

	/* Disable timer: */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_nsec = 0L;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = 0L;

	if( timer_settime(timerid->timerid, 0, &timer, NULL) < 0 )
	{
		return -1;
	}

	return 0;
}

int timerGet(timer* timerid, time_t* sec, long* nsec)
{
	struct itimerspec timer;

	if( timer_gettime(timerid->timerid, &timer) < 0)
	{
		return -1;
	}

	*sec = timer.it_value.tv_sec;
	*nsec = timer.it_value.tv_nsec;

	return 0;
}

int isTimerExpired(timer* timerid)
{
	time_t sec;
	long nsec;

	if( timerGet( timerid, &sec, &nsec) < 0 )
	{
		return 1; /* on an error, assume timer has expired so we don't block forever */
	}

	if( (sec > 0) || (nsec > 0) )
	{
		return 0;
	}

	return 1;
}

/* //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// */
int periodicTimerCreate(periodicTimer* pt, long threadId, int signalNum)
{
	/*
	// Create monotonic clock. It does not rely on the
	// system-wide real-time "settable" clock.
	// The monotonic clock is unsettable and is
	// good for relative timing...
	// Send a signal on timer expiration:
	*/
	struct sigevent sevp;
	if( threadId >= 0 )
	{	
		/*
		// If thread id is given notify only 
		// that specific thread.
		*/
		sevp.sigev_notify = SIGEV_THREAD_ID;
		sevp._sigev_un._tid = threadId; 
		/*
		// same as sigev_notify_thread_id
		// see here: http://stackoverflow.com/questions/16826898/error-struct-sigevent-has-no-member-named-sigev-notify-thread-id
		*/
	}
	else
	{
		/* Otherwise notify the entire process. */
		sevp.sigev_notify = SIGEV_SIGNAL;
	}

	/* Send an alarm signal: */
	pt->signum = signalNum;
	sevp.sigev_signo = pt->signum;

	/* Allow alarm signal to be caught: */
	sigemptyset(&pt->set);
	sigaddset(&pt->set, pt->signum);
	sigprocmask(SIG_BLOCK, &pt->set, NULL);

	if( timer_create(CLOCK_MONOTONIC, &sevp, &pt->timerid) < 0)
	{
		return -1;
	}

	pt->enabled = 1;

	return 0;
}

int periodicTimerDelete(periodicTimer* pt)
{
	/* Disallow alarm signal to be caught: */
	sigemptyset(&pt->set);
	sigaddset(&pt->set, pt->signum);
	sigprocmask(SIG_UNBLOCK, &pt->set, NULL);

	return timer_delete(pt->timerid);
}

int periodicTimerSet(periodicTimer* pt, const time_t sec, const long nsec)
{
	struct itimerspec timer;

	/* Enable interval timing, we want a "periodic" timer: */
	timer.it_interval.tv_sec = sec;
	timer.it_interval.tv_nsec = nsec;

	/* Set timer to expire at desired relative time: */
	timer.it_value.tv_sec = sec;
	timer.it_value.tv_nsec = nsec;

	if( timer_settime(pt->timerid, 0, &timer, NULL) < 0 )
	{
		return -1;
	}

	return 0;
}

int periodicTimerSet_us(periodicTimer* pt, const long usec)
{
	struct itimerspec timer;

	/* Enable interval timing, we want a "periodic" timer: */
	timer.it_interval.tv_sec = usec / 1000000;
	timer.it_interval.tv_nsec = (usec % 1000000) * 1000;

	/* Set timer to expire at desired relative time: */
	timer.it_value.tv_sec = usec / 1000000;
	timer.it_value.tv_nsec = (usec % 1000000) * 1000;

	if( timer_settime(pt->timerid, 0, &timer, NULL) < 0 )
	{
		return -1;
	}

	return 0;
}

int periodicTimerUnset(periodicTimer* pt)
{
	struct itimerspec timer;

	/* Disable timer: */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_nsec = 0L;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = 0L;

	if( timer_settime(pt->timerid, 0, &timer, NULL) < 0 )
	{
		return -1;
	}

	return 0;
}

int periodicTimerGet(periodicTimer* pt, time_t* sec, long* nsec)
{
	struct itimerspec timer;

	if( timer_gettime(pt->timerid, &timer) < 0)
	{
		return -1;
	}

	*sec = timer.it_value.tv_sec;
	*nsec = timer.it_value.tv_nsec;

	return 0;
}

int periodicTimerIntervalGet(periodicTimer* pt, time_t* sec, long* nsec)
{
	struct itimerspec timer;

	if( timer_gettime(pt->timerid, &timer) < 0)
	{
		return -1;
	}

	*sec = timer.it_interval.tv_sec;
	*nsec = timer.it_interval.tv_nsec;

	return 0;
}

int isPeriodicTimerSet(periodicTimer* pt)
{
	time_t sec;
	long nsec;

	if( periodicTimerIntervalGet( pt, &sec, &nsec ) < 0 )
	{
		return 0;
	}

	if( (sec > 0) || (nsec > 0) )
	{
		return 1;
	}

	return 0;
}

void periodicTimerEnable(periodicTimer* pt)
{
	pt->enabled = 1;
}

void periodicTimerDisable(periodicTimer* pt)
{
	pt->enabled = 0;
}

int isPeriodicTimerEnabled(periodicTimer* pt)
{
	return pt->enabled;
}

int periodicTimerWait(periodicTimer* pt, long sec, long nsec)
{
	struct timespec timeout;
	int sig;

	if(!pt->enabled)
		return pt->signum;

	if( sec >= 0 || nsec >= 0 )
	{
		timeout.tv_sec = sec;
		timeout.tv_nsec = nsec;

		if( (sig = sigtimedwait(&pt->set, NULL, &timeout)) < 0 )
		{
			if( errno == EAGAIN )
			{
				return 0;
			}

			return -1;
		}

		return sig;
	}
	else
	{
		return sigwaitinfo(&pt->set, NULL);
	}

	return 0;
}

/* ///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// */
int stopWatchStart(stopWatch* sw)
{ 
	return clock_gettime(CLOCK_MONOTONIC, &sw->start_t); 
}

int stopWatchStop(stopWatch* sw)
{ 
	return clock_gettime(CLOCK_MONOTONIC, &sw->stop_t); 
}

long stopWatchReport(stopWatch* sw)
{ 
	return (sw->stop_t.tv_sec*1000000000 + sw->stop_t.tv_nsec) - (sw->start_t.tv_sec*1000000000 + sw->start_t.tv_nsec);
}

/* ///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// */
int rt_sleep(const time_t sec, const long nsec)
{
	struct timespec ts;
	int status;
    ts.tv_sec = sec;
    ts.tv_nsec = nsec;

    do {
        status = clock_nanosleep(CLOCK_MONOTONIC, 
        							0, &ts, &ts);
    } while(status == EINTR); 
    /* Continue sleeping if interupted by signal handler */
    return status;
}

int rt_sleep_us(const long usec)
{
	struct timespec ts;
	int status;
	ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;

    do {
        status = clock_nanosleep(CLOCK_MONOTONIC, 
        							0, &ts, &ts);
    } while(status == EINTR); 
    /* Continue sleeping if interupted by signal handler */
    return status;
}

/* ///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////// */
extern int rt_time( time_t* sec, long* nsec )
{
	struct timespec timeSpec;
	int ret = clock_gettime(CLOCK_REALTIME, &timeSpec);
	*sec  = timeSpec.tv_sec;
	*nsec = timeSpec.tv_nsec;
	return ret;
}

extern int rt_time_ts( struct timespec* timeSpec )
{
	return clock_gettime(CLOCK_REALTIME, timeSpec);
}

/* return nanosecond timestamp */
unsigned long rt_time_ns()
{
	struct timespec timeSpec;
	clock_gettime(CLOCK_REALTIME, &timeSpec);
	return (timeSpec.tv_sec*1000000000 + timeSpec.tv_nsec);
}

unsigned long rt_time_tsns(struct timespec* timeSpecStruct)
{
	return (timeSpecStruct->tv_sec*1000000000 + timeSpecStruct->tv_nsec);
}

/* ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////// */
void usec_2_sec_nsec(long usec, time_t* sec, long* nsec)
{
	*sec = usec / 1000000;
	*nsec = (usec % 1000000) * 1000;
}
