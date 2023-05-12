# <b>Project 2 Report | Lucas Nguyen, Prabhjot Singh</b>

## <u>Implementation of Queue API:</u>

To implement a first-in-first-out queue, we utilized a linked list. 
A queue struct keeps track of pointers to the first inserted and last inserted 
items of the list as well as the total number of items inserted. The linked list
is implemented through a node struct where each node holds a void * untyped 
pointer to hold the address of various types of data.

Our queue API is then composed of several different functions. A creation
function that allocates memory for a queue and initializes it as empty. A 
destroy function frees the allocated memory of a given queue only if that queue
is empty. An enqueue function allocates memory for a non-null item to be 
inserted into a non-null queue by using the pointers that keep track of the
first item, the head, and the last item, the tail, of the queue. If the queue is
empty, both pointers will point to this non-null item, but if the queue has items
in it, this non-null item becomes the tail by adjusting the tail to point to the
item as the next item in the linked list. A dequeue function grabs and removes
the head of a non-empty non-null queue while having a given non-null pointer,
data, point to it. Both enqueue and dequeue functions adjust the count of the
queue accordingly. A delete function iterates through a non-null queue until it
finds the address that a given non-null pointer points to, and removes it from 
the queue. If the head is the part of the queue to be deleted, this function
acts as if it were the dequeue function. Otherwise, the function utilizes two 
local variables to parse the queue's linked list. If the item is not found, the
function fails and returns -1, but if it is found, one of the local variables
represent the item before the one to be deleted and the function adjusts the item
before's link to the item after the one to be deleted so that the item is 
deleted and the linked list remains intact. This delete function also adjusts
the count of the queue upon successful deletion. An interate function allows for
a given non-null function to execute on each of the items in a non-null 
non-empty queue. It utilizes a local variable to iterate through the queue's 
linked listfrom the head to the tail. Finally, a length function returns the 
length of the  dqueue, which is also the overall count.

To test the queue, we came up with many different test cases that 
ensure if a user makes a mistake, it will be accounted for. Some of the simpler
cases were to check if every function of the queue worked properly just  
enqueueing data, dequeueing data, and then deleting data from the queue. All of
these functions worked properly along with destroying an empty queue, iterating 
through all the values, and getting the queue length. After this we moved on to 
checking more complex scenarios such as trying to enqueue data that is NULL or 
if the queue itself was NULL. Our most complex scenario was trying to reuse a 
single queue that had values deleted, dequeued, or iterated through to
verify that there were no issues.

## <u>Implementation of User-Level Thread API:</u>

To implement a user-level thread API, we utilized the uthread_tcb struct to
keep track of each thread's thread control block which consists of a pointer to
the top of the thread's stack, a pointer to its context, and an integer that
represents what state the thread is in, ready, running, exited, or blocked, and
a thread struct keeps a pointer to the currently executing thread's tcb, a ready
queue that holds the tcbs of threads that are ready to execute and a zombie 
queue that holds the tcbs of threads that have exited. A global thread pointer
allows access for all functions to the current thread's tcb as well as the ready
and zombie queues.

The thread API consists of several functions. The current function returns the
current thread's tcb. The yield function utilizes two local tcb structs to keep 
track of the current thread's tcb, which is now the previous thread and is
enqueued to the ready queue, and the next thread's tcb, which will be the 
current thread and is dequeued from the ready queue. The global thread pointer's
tcb is then adjusted to this next thread's tcb and then these two tcb's are 
context switched. In context switching, we activate the next thread's context and 
then we save the previous thread's context. The exit function is very similar to 
the yield function where instead of enqueueing the current thread's tcb to the
ready queue, we enqueue it to the zombie queue instead. This is because when the
exit function is called, it means that the current thread that was running has 
finished its execution and so it has now entered a zombie state where it no 
longer will be running again. Adding it to the zombie queue allows us to keep 
track of the thread and be able to free it later on. The create function is for
creating new threads which is done by creating a new tcb struct and then 
initializing the members. The stack pointer member is initialized by allocating
space to the size of the stack. The context is initialized by a function called 
uthread_ctx_init that takes in the context, stack pointer, function, and 
arguments of the thread. This function sets the context's stack and makes 
the context using the thread library. The state member is then initialized by 
setting its state to the ready state and adding it to the ready queue. Then in the 
run function, the first function that is called for the thread API, we similarly
set up the idle thread like in the create function but instead, we set its 
context up manually because there is no function or arguments to go with the idle
thread. As this is the first function that is run, the zombie and ready queues 
are created and then the first thread is created with the create function. We 
finally begin the threading process by using a while loop that continues 
forever, until the ready queue is empty which means that there are no more 
threads ready to be used. In this loop, the main use is to call the yield 
function when the idle thread is running, which is done to switch to the next 
thread that is ready to be used. We also have another while loop for when the 
zombie queue is not empty. As previously mentioned, the zombie queue stores all
the exited threads which always us to easily free the thread's memory in the 
loop. The last two functions of the thread API were used with 
semaphores. The block function would switch from the currently running thread to 
the next available thread as the current thread would be in a blocked state and 
so it cannot run anymore. The unblock function would take any unblocked threads 
and set their states back to ready and enqueue them into the ready queue.

## <u>Implementation of Semaphore API:</u>

To implement the semaphore API, we utilized a semaphore struct that 
contained the count of available resources and the queue of blocked threads as 
its members. When the available resources count became zero that meant that any
threads that need to use those resources would be put into the blocked queue. 
Once the resource count got back up that meant that the blocked threads would be
allowed to be run again and so they were put back into the ready queue.

To implement the semaphore API we had to create several functions as well as 
utilize some functions from the thread API. The first function was the create
function that allocated memory for a new semaphore and initialized it with an 
empty queue and the number of resources that were given. The destroy function
is used when the semaphores are no longer needed and there are no more threads 
in the blocked state which means we can destroy the semaphore. The down function 
is used whenever a resource is needed by a thread and in this function we also
check a particular case of when a blocked thread becomes free we make sure that 
if the resource it needs is taken again by another thread, it continues to be 
blocked. This is done by implementing a while loop that continuously loops when 
the number of available resources is zero and utilizing the thread API block 
function. In this while loop, if a thread needs to access a resource that is 
currently being used, the thread is put into the blocked queue. The while loop
also ensures that in that case scenario, if a thread is no longer blocked,
the number of resources needs to be more than one before it can exit the loop.
Once outside of the loop, it means that resources are available to the thread 
and it will utilize them which means we need to decrement the count. 
In the up function, it is used when a resource has been freed by a thread and so
other threads can utilize it. In the function, we first increment the amount of 
available resources and then if there are threads in the blocked queue, we 
unblock them using the unblock function from the thread AP. 

## <u>Implementation of Preemption:</u>

To implement preemption, we utilized timers that would activate an 
alarm every one hundred hertz. The alarm then would interrupt the current thread
running, moving on to the next one. The current thread would then reenter the 
ready queue and the process would repeat. 

Preemption was done by creating several functions as well as including those 
functions in the thread API. The start function was where the timer, alarm, and 
interrupt handler were initialized. In the start function, the interrupt handler
was assigned to a function called the thread yield function. This was 
because interrupts only happened when one hundred hertz had passed and the 
signal, SIGVTALRM, went off which meant the next thread should run. For the 
timer to go off, we set the interval and initial values of a virtual 
timer to be 10000 microseconds which meant that the alarm would go off after 
10000 microseconds and every 10000 afterward. The reason behind choosing 10000 
microseconds was because one hundred hertz correlates to 0.01 seconds. The stop 
function resets the interrupt handler, timer, and alarm. In the function, 
whenever SIGVTALRM goes off the handler now does whatever was the default, the 
timer is also set to zero for the interval and initial values which stops the 
alarm. The disable and enable functions are very similar where the disable 
function stops preemption by blocking the SIGVTALRM signal and enable unblocks
SIGVTALRM which allows for preemption to continue. All functions in preemption 
make sure that preemption was actually enabled and if it was not, the function
would simply return. 

When using preemption, we had to be careful that when we switch threads the
critical sections were not being accessed or any shared resources were being 
used between multiple threads. Therefore as previously mentioned, we had to 
include some of the preemption functions in the thread API. These functions 
namely were the disable and enable functions. We would disable preemption 
anytime before entering a critical section which included the create, yield, and
exit functions as they involved global resources such as the queue and tcbs of 
the threads. In the yield and exit functions, we did not use the enable function 
as it is already called when we context switch in those functions but we do use 
enable in the create function. 

To test whether preemption worked or not, we created a simple program called 
test_preempt. The goal of the program was to test preemption so to do 
so we first created a thread called thread1 that would create another thread 
called thread2. Thread1 would continuously execute a while loop that had a 
global boolean variable as its parameter. Thread2 was where that boolean 
variable was eventually set to false which meant that if preemption works, then
thread2 would be executed after thread1 is put back into the ready queue and so
the while loop would stop and thread1 would exit. This was shown to be 
successful for us and we made it so that thread2 was called through preemption 
and not through an explicit call. 
