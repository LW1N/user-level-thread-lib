#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

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
// Signal variable
sigset_t sig;
// Keep track of if preemption was asked for
bool preemption = false;

void yield_func(int signum) 
{
  // void arg parameter
  (void)signum;

  // Disable preemption so uthread_yield() can run
  preempt_disable();
  uthread_yield();
}

void preempt_disable(void)
{
  // If preemption wasn't asked for, do nothing
	if(!preemption) {
		return;
	}
  // Blocks the signal set in sig which is SIGVTALRM
  sigprocmask(SIG_BLOCK, &sig, NULL);
}

void preempt_enable(void)
{
  // If preemption wasn't asked for, do nothing
	if(!preemption) {
		return; 
	}
  // Unblocks the signal set in sig which is SIGVTALRM
  sigprocmask(SIG_UNBLOCK, &sig, NULL);
}

void preempt_start(bool preempt)
{
  // If preemption wasn't asked for, do nothing
	if(!preempt) {
		return;
	}
	// Keep track of whether or not preemption was asked for
	preemption = true;
	// Install signal handler for SIGVTALRM
	memset(&sa, 0, sizeof(sa));

	// Set signal handler and flags for SIGVTALRM
	sa.sa_handler = &yield_func;
  sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL);
  // Set signal to be empty
  sigemptyset(&sig);
  // Fill signal with SIGVTARLM
  sigaddset(&sig, SIGVTALRM);
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
  // If preemption wasn't asked for, do nothing
  if (!preemption) {
      return;
  }

	memset(&sa, 0, sizeof(sa));

  // Revert to default handler
  sa.sa_handler = SIG_DFL;
  sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL);
  // Setting it_value and interval to 0 stops the alarm
  timer.it_value.tv_sec = 0; 
  timer.it_value.tv_usec = 0; 
  timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
  // Reset timer as nothing
  setitimer(ITIMER_VIRTUAL, &timer, NULL); 

  // set preemption to false
  preemption = false;
}

