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

// 0 -> ready to run , 1 -> running , -1 -> exited
enum state{ready = 0, running = 1, exited = -1};

struct uthread_tcb {
	/* TODO Phase 2 */
	// Holds pointer to thread's stack area
	void *stackpointer;
	// Pointer to thread's context(set of registers)
	uthread_ctx_t* context;
	// Holds current state value
	int s; 
};

// Global tcb pointer representing the current thread
struct uthread_tcb *cthread;
// Global queue pointer representing the thread ready queue
struct queue *readyq;

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
  // pointer to current runing thread
  
}

void uthread_yield(void)
{
	/* TODO Phase 2 */
	// Allocate memory for next thread in queue
	struct uthread_tcb *nthread;
	nthread = malloc(sizeof *nthread);
	// Grab the next ready thread from the ready queue
	queue_dequeue(readyq, (void**)&nthread);
	// Switch execution context of current thread and next thread
	uthread_ctx_switch(cthread->context, nthread->context);
	// Set cthread to ready state and put into the ready queue
	cthread->s = ready;
	queue_enqueue(readyq, cthread);
	// Change current thread to next thread
	cthread = nthread;
	// Current thread should always be in running state
	cthread->s = running;
	// Free allocated memory for nthread
	free(nthread);
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
	cthread->s = exited;

}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	// Allocate memory for new tcb
	struct uthread_tcb *newtcb;
	newtcb = malloc(sizeof *newtcb);
	// Return -1 if malloc fails
	if(!newtcb){
		free(newtcb);
		return -1;
	}
	// Allocate a stack to the thread and store pointer to the stack in stackpointer
	newtcb->stackpointer = uthread_ctx_alloc_stack();
	// Initialize NULL context pointer in order to properly retrieve context pointer for stack
	newtcb->context = NULL;
	
	
	// Return -1 if pointer to stack isn't properly allocated or context is not properly initialized
	if(!newtcb->stackpointer || uthread_ctx_init(newtcb->context, newtcb->stackpointer, func, arg)){
		free(newtcb->stackpointer);
		return -1;
	}

	// Set state to ready
	newtcb->s = ready;
	// Enqueue new thread to the ready queue
	queue_enqueue(readyq, newtcb);
	// Return 0 on success
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
  if (preempt) {
    // do something
  }

  // set up initial running tcb
  struct uthread_tcb *idle_tcb;
  idle_tcb = malloc(sizeof *idle_tcb);
  if (!idle_tcb) {
    return -1;
  }

  // allocate stack and context for tcb
  idle_tcb->stackpointer = uthread_ctx_alloc_stack();
  idle_tcb->context = NULL;
  if(!idle_tcb->stackpointer){
		free(idle_tcb->stackpointer);
		return -1;
	}
  int init_context_result = uthread_ctx_init(idle_tcb->context, idle_tcb->stackpointer, func, arg);
  if (init_context_result == -1) {
    return -1;
  }

  // Create the ready queue
  readyq = queue_create();
  queue_enqueue(readyq, &idle_tcb);
  // create inital thread and check if it is valid
  if (uthread_create(func, arg) == -1) {
    return -1;
  }

  // create a struct for the next thread
  struct uthread_tcb *nthread;
	nthread = malloc(sizeof *nthread);

  // only return once all threads have been excuted
  while (!queue_length(readyq)) {
    // get the first thread from the queue
    queue_dequeue(readyq, (void**)&nthread);
    // save previous threads context and then activate next threads context
    uthread_ctx_switch(cthread->context, nthread->context);
    // check if current thread has not been exited and if so add it back to queue
    if (cthread->s != exited) {
      queue_dequeue(readyq, cthread);
    }
    // set the current thread to be the next thread
    cthread = nthread;
  }
  // destroy queue once done
  queue_destroy(readyq); 
  return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
}

