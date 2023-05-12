#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

// Node struct to represent linked list of items
struct node{
	// Data holds address of enqueued variable
	void *data;
  // Next holds the next node in the queue
	struct node* next;
};

typedef struct node node;

// Queue struct to keep track of first and last items in list
struct queue {
	// Count is size of queue(# of items in queue)
	int count;
  // Head is first enqueued
	node *head;
  // Tail is last enqueued
	node *tail;
};

queue_t queue_create(void)
{
	// Allocate memory for newqueue
	struct queue *newqueue;
	newqueue = malloc(sizeof *newqueue);

	// If malloc fails, return NULL
	if(!newqueue){
		return newqueue;
	}

	// Initialize and return empty queue
	newqueue->count = 0;
	newqueue->head = NULL;
	newqueue->tail = NULL;
	return newqueue;
}

int queue_destroy(queue_t queue)
{
	// Return -1 if queue is NULL or not empty 
	if(queue == NULL || queue->count != 0){
		return -1;
	}

	// Free the empty queue and return 0
	free(queue);
	queue = NULL;
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	// If queue/data are NULL, return -1
	if(queue == NULL || data == NULL){
		return -1;
	}

	// Allocate space for the new item to be added to queue
	node *new_item;
	new_item = malloc(sizeof(node));
	new_item->data = data;
	new_item->next = NULL;

	// If malloc fails, return -1
	if(new_item == NULL){
		free(new_item);
		return -1;
	}
	
	// If enqueueing to an empty queue, new_item -> first in queue
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

	// Increment count to keep track of size of queue, return 0
	queue->count++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	// Remove first item in queue and assign to data
	// Return -1 if queue or data is NULL and 0 if succesful

	// If queue/data is NULL or the queue is empty
	if (queue == NULL || *data == NULL || queue->head == NULL) {
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
	// Delete first instance of data found 

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

	// Iterate through list until the data is found or at the end
	while (target != NULL && target->data != data) {
		// Grab the previous node
		before = target;
		// Make target equal to next node in ll
		target = target->next; 
	}

	// If data wasn't found, return -1
	if (target == NULL || target->data != data) {
		return -1;
	}


	// We can 'delete' the target by skipping over it in the linked list
	// Instead of before-target-next -> before-next
	before->next = target->next;

	// Delete node by setting it equal to NULL
	target->data = NULL;
	target->next = NULL;

	// Decrement count of queue and return 0
	queue->count--;
	return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
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

	// Return 0 upon success
	return 0;
}

int queue_length(queue_t queue)
{
	// If queue is NULL, return -1
	if(queue == NULL){
		return -1;
	}

	// return count, which is length of queue
	return queue->count;
}

