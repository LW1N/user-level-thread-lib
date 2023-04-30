#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

/* Delete simple */
void test_delete(void){
	int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
	size_t i;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete ***\n");

	// Create a queue representing data
	q = queue_create();
	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
		queue_enqueue(q, &data[i]);
	
	// Delete 2
	queue_delete(q, &data[1]);
	TEST_ASSERT(queue_length(q) == 9);

	// Delete 1
	queue_delete(q, &data[0]);
	int *ptr;

	// dequeues 3 now
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(*ptr == 3);
}

/* Iterator test */
static void iterator_inc(queue_t q, void *data)
{
	int *a = (int*)data;
	// Delete 
	if (*a == 42)
		queue_delete(q, data);
	else
		*a += 1;
}

void test_iterator(void)
{
	queue_t q;
	int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
	size_t i;
	
	fprintf(stderr, "*** TEST queue_iterate ***\n");
	/* Initialize the queue and enqueue items */
	q = queue_create();
	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
		queue_enqueue(q, &data[i]);

	/* Increment every item of the queue, delete item '42' */
	queue_iterate(q, iterator_inc);
	TEST_ASSERT(data[0] == 2);
	TEST_ASSERT(queue_length(q) == 9);
}

/* Complex test */
static void simple_iteration(queue_t q, void *data) {
	// checks if queue length is 1
	TEST_ASSERT(queue_length(q) == 1);
	// overwrite the data to be 50
	int *a = (int*)data;
	*a = 50;
}

static void test_complex(void) 
{
	queue_t q;
  int data[] = {1, 2, 3, 4};
	size_t i;
	int *ptr;

	fprintf(stderr, "*** TEST queue_complex ***\n");
	q = queue_create();
	// add data to queue
	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
		queue_enqueue(q, &data[i]);

	// dequeues 1 
	queue_dequeue(q, (void**)&ptr);
	// checks if data is 1
	TEST_ASSERT(*ptr == 1);
	// makes sure length is 3
	TEST_ASSERT(queue_length(q) == 3);
	// makes sure that if we try to delete 1 from the queue it returns -1 as its already been deleted
	TEST_ASSERT(queue_delete(q, (int*) 1) == -1);
	// dequeue the next two values of the queue 
	queue_dequeue(q, (void**)&ptr);
	queue_dequeue(q, (void**)&ptr);
	// makes sure that if we try to delete the queue it returns -1
	TEST_ASSERT(queue_destroy(q) == -1);
	// make the queue empty
	queue_dequeue(q, (void**)&ptr);
	// make sure the queue is empty
	TEST_ASSERT(queue_length(q) == 0);

	int s = 2;
	// now requeue the queue with 2
	queue_enqueue(q, &s);
	// test the iteration function
	queue_iterate(q, simple_iteration);
	// now get the value in the queue which should be 50
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(*ptr == 50);
	// now delete the queue and make sure we return 0
	TEST_ASSERT(queue_destroy(q) == 0);
}


int main(void)
{
	test_create();
	test_queue_simple();
	test_iterator();
	test_delete();
	test_complex();

	return 0;
}
