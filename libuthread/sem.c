#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

// Semaphore struct, internal count and waiting list(queue)
struct semaphore {
	size_t count;
	queue_t queue;
};

sem_t sem_create(size_t count)
{
	// Allocate memory for new semaphore
	struct semaphore *newsem;
	newsem = malloc(sizeof *newsem);

	// If malloc fails, return NULL
	if(!newsem){
		return newsem;
	}

	// Initialize semaphore w/ empty queue and given count
	newsem->count = count;
	newsem->queue = queue_create();
	return newsem;
}

int sem_destroy(sem_t sem)
{
	// If sem is NULL or there are threads still waiting, return -1
	if (!sem || queue_length(sem->queue)) {
		return -1;
	}

	// Otherwise, destroy the queue and free given semaphore
	queue_destroy(sem->queue);
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	// Return -1 if given semaphore is NULL
	if(!sem){
		return -1;
	}
	
	// If count is 0(trying to access same var), block thread
	if (sem->count == 0){
		// Grab current thread
		// struct uthread_tcb *ntcb = uthread_current();
		// Add thread queue(waiting list)
		queue_enqueue(sem->queue, uthread_current());
		// Block thread
		uthread_block();
	}

	// Decrement semaphore count, and return 0
	sem->count -= 1;
	return 0;
}

int sem_up(sem_t sem)
{
	// Return -1 if given semaphore is NULL
	if(!sem){
		return -1;
	}

	// Increment semaphore count
	sem->count += 1;

	// If the queue(waiting list) is empty, do nothing else
	if(!sem->queue || !queue_length(sem->queue)){
		return 0;
	}

	// Unblock oldest blocked thread in waiting list
	struct uthread_tcb *ntcb;
	ntcb = malloc(sizeof ntcb);	
	// Grab oldest thread in waiting list
	queue_dequeue(sem->queue, (void**)&ntcb);
	if(!ntcb){
		return 0;
	}
	// Unblock thread and return 0
	uthread_unblock(ntcb);

	return 0;
}

