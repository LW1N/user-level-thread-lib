#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

// Global timer variables
struct sigaction sa;
struct itimerval timer;
bool preemption = false;

void preempt_disable(void)
{
	/* TODO Phase 4 */
	if(!preemption){
		return;
	}
	sigprocmask(SIG_BLOCK, &blocksigs, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	if(!preemption){
		return;
	}
	sigprocmask(SIG_UNBLOCK, &blocksigs, NULL);
}

void preempt_start(bool preempt)
{
	/* TODO Phase 4 */
	if(!preempt){
		return;
	}
	
	preemption = true;

	// Install signal handler for SIGVTALRM
	memset(&sa, 0, sizeof(sa));

	// Set signal handler for SIGVTALRM
	sa.sa_handler = &timer_handler;
	sa.sa_flags = SA_NOMASK | SA_ONSTACK;
	// sigemptyset(&sa.sa_mask);
	sigaction(SIGVTALRM, &sa, NULL);

	// Set up timer to trigger SIGVTALRM every HZ per second
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = HZ;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = HZ;

	// Start a virtual timer
	setitimer(ITIMER_VIRTUAL, &timer, NULL);

}

void preempt_stop(void)
{
	/* TODO Phase 4 */
	
}

