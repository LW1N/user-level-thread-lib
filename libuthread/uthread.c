#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

/* Size of the stack for a thread (in bytes) */
#define UTHREAD_STACK_SIZE 32768

// 0 -> ready to run , 1 -> running , 2 -> blocked, -1 -> exited
enum state{ready = 0, running = 1, blocked = 2, exited = -1};

// Thread TCB struct to keep track of each thread's components
struct uthread_tcb {
	/* TODO Phase 2 */
	// Holds pointer to thread's stack area
	void *stackpointer;
	// Pointer to thread's context(set of registers)
	uthread_ctx_t *context;
	// Holds current state value
	int s;
	// Holds current thread number
	int threadNum;
};

// Global int to keep track of total number of threads
int numThreads = 0;

// Thread struct to keep track of current thread tcb
// As well as ready, zombie, and blocked queues
struct thread{
	struct uthread_tcb *tcb;
	struct queue *readyq;
	struct queue *zombieq;
	struct queue *blockedq;
};

// Global thread pointer representing the current thread
struct thread *cthread;

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	// pointer to current runing thread
	return cthread->tcb;
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	// Next thread in queue
	struct uthread_tcb *ntcb;
	// Previous running thread
	struct uthread_tcb *ptcb;
	// Allocate memory for next thread in queue
	ntcb = malloc(sizeof *ntcb);
	// If malloc fails, return
	if(!ntcb){
		free(ntcb);
		return;
	}

	// Keep track of previous thread as ptcb
	ptcb = cthread->tcb;
	// Grab the next ready thread from the ready queue
	queue_dequeue(cthread->readyq, (void**)&ntcb);
	// Set current running thread to ready
	cthread->tcb->s = ready;
	// Enqueue previous thread back in to ready queue
	queue_enqueue(cthread->readyq, cthread->tcb);
	// Next thread is now running
	ntcb->s = running;
	// Current thread is going to be next thread
	cthread->tcb = ntcb;
	// Switch from previous to next thread context
	uthread_ctx_switch(ptcb->context, cthread->tcb->context);
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	// Next thread in queue
	struct uthread_tcb *ntcb;
	// Previous running thread
	struct uthread_tcb *ptcb;
	// Keep track of previous thread as ptcb
	ptcb = cthread->tcb;
	// Grab the next ready thread from the ready queue
	queue_dequeue(cthread->readyq, (void**)&ntcb);
	// Set previous thread as exited
	cthread->tcb->s = exited;
	// Enqueue previous exited thread into the zombie queue
	queue_enqueue(cthread->zombieq, cthread->tcb);
	// Next thread will be running
	ntcb->s = running;
	// Set current thread as next thread
	cthread->tcb = ntcb;
	// Switch to next thread context
	uthread_ctx_switch(ptcb->context, cthread->tcb->context);
}

int uthread_create(uthread_func_t func, void *arg)
{
	//TESTING
	numThreads++;

	// Allocate memory for new thread tcb
	struct uthread_tcb *newtcb;
	newtcb = malloc(sizeof *newtcb);
	// Return -1 if malloc fails
	if(!newtcb){
		free(newtcb);
		return -1;
	}
	// Allocate stack and context pointers for new thread tcb
	newtcb->stackpointer = uthread_ctx_alloc_stack();
	newtcb->context = malloc(sizeof(uthread_ctx_t));
	
	// Check in case of memory allocation failure
	if(!newtcb->stackpointer || !newtcb->context) {
		free(newtcb->stackpointer);
		free(newtcb->context);
		return -1;
	}

	// Return -1 if context is not properly initialized
	if(uthread_ctx_init(newtcb->context, newtcb->stackpointer, func, arg)){
		return -1;
	}

	// TESTING
	newtcb->threadNum = numThreads;

	// Set state to ready
	newtcb->s = ready;
	// Enqueue new thread to the ready queue
	queue_enqueue(cthread->readyq, newtcb);
	// Return 0 on success
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	
    // Check for preemption, set up if needed
	preempt_start(preempt);
	

  	// Set up and allocate space for idle tcb
	struct uthread_tcb *idle_tcb;
	// Allocate space for the current thread & idle thread tcb
	cthread = (struct thread *)malloc(sizeof(struct thread));
	idle_tcb = malloc(sizeof *idle_tcb);

	// Check in case of malloc failure
	if (!idle_tcb || !cthread) {
		free(idle_tcb);
		free(cthread);
		return -1;
	}
	
	// allocate stack and context for idle thread tcb
	idle_tcb->stackpointer = uthread_ctx_alloc_stack();
	idle_tcb->context = malloc(sizeof(uthread_ctx_t));
	// Check for error in memory allocation
	if(!idle_tcb->stackpointer || !idle_tcb->context) {
		free(idle_tcb->stackpointer);
		free(idle_tcb->context);
		return -1;
	}

	// Grab and store current context in idle_tcb
	if(getcontext(idle_tcb->context)){
		// If getcontext fails, return -1
		return -1;
	}

	// Idle thread with context size and stackpointer
	idle_tcb->context->uc_stack.ss_sp = idle_tcb->stackpointer;
	idle_tcb->context->uc_stack.ss_size = UTHREAD_STACK_SIZE;
	// Set to ready, -> will just sit in queue until other threads are done
	idle_tcb->s = ready;

	// TESTING
	idle_tcb->threadNum = 0;

	// Right now current thread is idle thread 
	cthread->tcb = idle_tcb;
	// Create queues for waiting, exited, and blocked threads
	cthread->readyq   = queue_create();
	cthread->zombieq  = queue_create();
	cthread->blockedq = queue_create();
	
	// Create first initial thread and put in ready queue
	if (uthread_create(func, arg)) {
		// Return -1 upon failure
		return -1;
	}

	// While loop to keep letting other threads do their thing
	while(queue_length(cthread->readyq)){
		// Anytime idle thread is put to execute, just yield to next in queue
		uthread_yield();
	}

	// While loop to free memory and collect exited threads
	while(queue_length(cthread->zombieq)) {
		// Grab exited thread and store in ntcb
		struct uthread_tcb *ntcb;
		queue_dequeue(cthread->zombieq, (void**)&ntcb);
		// Free thread stack and context pointers
		uthread_ctx_destroy_stack(ntcb->stackpointer);
		free(ntcb->context);
		free(ntcb);
	}

	// If preemption was required, call stop to end it
	if(preempt){
		preempt_stop();
	}
	// Free current(now idle) thread stack and context pointers
	uthread_ctx_destroy_stack(cthread->tcb->stackpointer);
	free(cthread->tcb->context);
	// Destroy queues once done
	queue_destroy(cthread->readyq); 
	queue_destroy(cthread->zombieq);
	queue_destroy(cthread->blockedq);
	// Free current thread
	free(cthread);
	return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
	// Next thread in ready queue
	struct uthread_tcb *ntcb;
	// Previous running thread
	struct uthread_tcb *ptcb;
	// Keep track of previous thread as ptcb
	ptcb = cthread->tcb;
	// Grab the next ready thread from the ready queue
	queue_dequeue(cthread->readyq, (void**)&ntcb);
	// Set previous thread as blocked
	cthread->tcb->s = blocked;
	// Enqueue now blocked thread into the blocked queue
	queue_enqueue(cthread->blockedq, cthread->tcb);
	// Next thread will be running
	ntcb->s = running;
	// Set current thread as next thread
	cthread->tcb = ntcb;
	// Switch to next thread context
	uthread_ctx_switch(ptcb->context, cthread->tcb->context);
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
	// Thread is no longer blocked
	uthread->s = ready;
	// Thread is put into ready queue and is avaiable to run again
	queue_enqueue(cthread->readyq, uthread);
	// Delete thread from blocked queue
	// queue_delete(cthread->blockedq, uthread);
}

