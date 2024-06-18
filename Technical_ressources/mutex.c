
/*
MUTEX

Previously : We left with the issue of race condition for
writing to the mails variable.
How to solve it ? With the mutex.

What's the mutex ? 
it's a sort of a lock around a section of code.


For example, what we could do :
We have a var. called lock set it to zero & set it to 1
whenever you're doing something, & set it to 0 whenever we're
not doing something.

So, first we check if lock is equal to 1, if so we wait until
the lock is zero.
If a thread tries to actually increment the mail var.,
it'll increment it but it will also set the lock to one.
So, if the 2nd thread comes in at the same time,  it's gonna
stop at that condition because the lock is gonna be set 1.
When it's done, the lock will be set again to zero & then the
thread 2 could come in and doing its tasks (here : read, increment & write).

How can we wait until the lock is equal to 0 ?
We don't really have to implement our own var with log
things.
Because, the pthread api actually has something like that that
we can use, & it's much safer.

To use it, we're declaring the pthread_mutex_t mutex; var.
First, we have to initialize it before creating the thread.
We're using : pthread_mutex_init();
1st param. : The address to that mutex.
2nd param. : Some attributes, if not NULL.

We also have to destroy the memory at the end that
has been allowed at the beginning. => pthread_mutex_destroy() : 
Param. : just takes the address to the mutex.

Now, the mutex is available to be used, so we go back to routine().
To check if the lock (var.) has  been locked & also to set up the
var. to 1, there is a function to do it. => pthread_mutex_lock() :
1st param. : address of mutex.

To check if the lock is not locked there is the pthread_mutex_unlock()
function.
1st param : Address to mutex.

What does this do ?
This thing of locking & unlocking, is basically protecting, ex.,
a part of code that you want to protect agains other threads
executing it at the same time.

Race condition can only be encountered on a multi-core processor.

*/

#include <pthread.h>

int	mails = 0; // Var. represents the nb of mails.
// int	lock = 0; // State of execution.
pthread_mutex_t	mutex;

void	*routine()
{
	for (int i = 0; i < 100000, i++)
	{
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);
	}
}

// Threads
int	main(int argc, char *argv[])
{
	pthread_t	t1, t2;
	pthread_mutex_init(&mutex, NULL);
	if (pthread_create(&t1, NULL, &routine, NULL) != 0)
		return (1);
	if (pthread_create(&t2, NULL, &routine, NULL) != 0)
		return (1);
	if (pthread_join(t1, NULL) != 0)
		return (2);
	if (pthread_join(t2, NULL) != 0)
		return (2);
	pthread_mutex_destroy(&mutex);
	printf("Nb of mails : %d\n", mails);
	return (0);
}