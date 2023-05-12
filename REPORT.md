# <b>Project 2 Report | Lucas Nguyen, Prabhjot Singh</b>

## <u>Implementation of Queue API:</u>
[------------------------------------------------------------------------------]
In order to implement a first in first out queue, we utilized a linked list. 
A queue struct keeps track of pointers to the first inserted and last inserted 
items of the list as well as the total number of items inserted. The linked list
is implemented through a node struct where each node holds a void * untyped 
pointer in order to hold the address of various types of data.

Our queue API is then composed of a several different functions. A creation
function that allocates memory for a queue and initializes it as empty. A 
destroy function frees the allocated memory of a given queue only if that queue
is empty. An enqueue function allocates memory for a non-null item to be 
inserted in to a non-null queue by using the pointers that keep track of the
first item, the head, and the last item, the tail, of the queue. If the queue is
empty, both pointer will point to this non-null item, but if the queue has items
in it, this non-null item becomes the tail by adjusting the tail to point to the
item as the next item in the linked list. A dequeue function grabs and removes
the head of a non-empty non-null queue while having a given non-null pointer,
data, point to it. Both enqueue and dequeue functions adjust the count of the
queue accordingly. A delete function iterates through a non-null queue until it
finds the address that a given non-null pointer points to, and removes it from 
the queue. If the head is the part of the queue to be deleted, this function
acts as if it were the dequeue function. Otherwise the function utilizes two 
local variables to parse the queue's linked list. If the item is not found, the
function fails and returns -1, but if it is found, one of the local variables
represent the item before the one to deleted and the function adjusts the item
before's link to the item after the one to be deleted so that the item is 
deleted and the linked list remains intact. This delete function also adjusts
the count of the queue upon successful deletion. An interate function allows for
a given non-null function to execute on each of the items in a non-null 
non-empty
queue. It utilizes a local variable to iterate through the queue's linked list
from the head to the tail. Finally a length function returns the length of the
queue, which is also the overall count.

In order to test the queue, we came up with many different test cases that 
ensure if a user makes a mistake, it will be accounted for. Some of the simpler
cases was to check if every function of the queue worked properly just has 
enqueueing data, dequeueing data, and then deleting data from the queue. All of
these functions worked properly along with destroying an empty queue, iterating 
through all the values, and getting the queue length. After this we moved on to 
checking more complex scenarios such as trying to enqueue data that is NULL or 
if the queue itself was NULL. Our most complex sceanrio was trying to reuse a 
single queue that had values deleted, dequeued, or iterated through in order to
verify that there were no issues.

## <u>Implementation of User-Level Thread API:</u>
In order to implement a user-level thread API, we utilized  

## <u>Implementation of Semaphore API:</u>


## <u>Implementation of Preemption:</u>

