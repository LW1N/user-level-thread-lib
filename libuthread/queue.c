#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

// node struct to represent linked list of items
struct node{
	// void *data holds address of enqueued variable
	void *data;
	struct node* next;
};
typedef struct node node;

// queue struct to hold first and last items in ll
struct queue {
	/* TODO Phase 1 */
	// Count is size of queue(# of items in queue)
	// head is first enqueued, tail is last
	int count;
	node *head;
	node *tail;
};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
	// Allocate memory for newqueue
	struct queue *newqueue;
	newqueue = malloc(sizeof *newqueue);
	// If malloc fails, return NULL
	if(!newqueue){
		return newqueue;
	}
	// Initialize empty queue
	newqueue->count = 0;
	newqueue->head = NULL;
	newqueue->tail = NULL;
	return newqueue;
}

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	/* return -1 if queue is not empty or NULL */
	if(!queue || queue->count != 0){
		return -1;
	}

	/* deallocate the queue if it is empty and exists and return 0 */
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	// Add to end of queue
	node *new_item;
	new_item = malloc(sizeof(node));
	new_item->data = data;
	new_item->next = NULL;

	// If queue/data are NULL or malloc fails, return -1
	if(queue == NULL || data == NULL || new_item == NULL){
		return -1;
	}

	// If enqueueing to an empty queue, data -> first in queue
	// Could use tail or head
	if(queue->tail == NULL){
		queue->head = new_item;
		queue->tail = new_item;
		queue->count++;
		return 0;
	}

	// If queue not empty, append new item to end
	// Point tail to new node & new node becomes new tail
	queue->tail->next = new_item;
	queue->tail = new_item;

	// Increment count to keep track of size of queue
	queue->count++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	// remove first item in queue and assign to data
	// return -1 if queue or data is NULL and 0 if succesful

	// check if queue is NULL or the value of the pointer is null
	if (queue == NULL || *data == NULL) {
		return -1;
	}

	// store address of first item data in data
	*data = queue->head->data; 

	// if 1 item in queue, nullify queue after dequeueing
	if (queue->head->next == NULL) {
		queue->head = NULL;
		queue->tail = NULL;
		queue->count--;
		return 0;
	}
	
	// Grab new head(1 right after previous head)
	// Decrease count
	queue->head = queue->head->next;
	queue->count--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	// delete first instance of data found 
	// use pointers 

	// Check if queue or data is NULL
	if (queue == NULL || data == NULL) {
		return -1;
	}

	// If first item is to be deleted, same as dequeue
	if (queue->head->data == data) {
		queue->head = queue->head->next;
		queue->count--;
		return 0;
	}

	// Before keeps track of the node right before the one to be deleted
	// Target is node to be deleted
	node *before;
	node *target;
	target = queue->head;
	before = target;

	// Keep on iterating through linked list until the data is found or we are at the end
	while (target != NULL && target->data != data) {
		// grab the previous node
		before = target;
		// make temp equal to next node in ll
		target = target->next; 
	}

	// Data wasn't found
	if (target == NULL || target->data != data) {
		return -1;
	}


	// we can 'delete' the target by skipping over it in the linked list
	// Instead of before-target-next -> before-next
	before->next = target->next;

	// delete node by setting it equal to NULL
	target->data = NULL;
	target->next = NULL;
	queue->count--;

	return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
	// Check if queue or func is NULL
	if (queue == NULL || func == NULL) {
		return -1;
	}
	
	// Iterate through list, performing func on each iteration
	node *temp = queue->head;
	while (temp != NULL) {
		func(queue, temp->data);
		temp = temp->next;
	}

	return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	// If queue is NULL, return -1
	if(!queue){
		return -1;
	}

	// Else return count, which is length of queue
	return queue->count;
}

