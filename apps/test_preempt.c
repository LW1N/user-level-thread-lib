#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <uthread.h>

/* If preemption works, thread1 will not be in an infinite loop */

// Global flag variable to loop thread1
bool flag[2] = {true, true};

void thread2(void *arg)
{
	(void)arg;

  // Stop thread 1 looping infinitely
  if (flag[0]){
    flag[1] = false;
  }
  printf("thread2\n");
}

void thread1(void *arg)
{
  (void)arg;

	uthread_create(thread2, NULL);
  // Have thread1 sleep as long as flag[1] is true
  // Thread1 will hold until thread2 finishes executing
  while(flag[1]) {
    sleep(0.01);
  } 
  printf("thread1\n");
}

int main(void)
{
	uthread_run(true, thread1, NULL);

  printf("thread1 and thread2 have executed!\n");
	return 0;
}
