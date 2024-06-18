
/*
CREATE THREADS IN A LOOP

The different threads we've created, we'll create them in
an array : pthread_t th[4];
=> Where the thread handles are going to be stored.

We create our loop, with an index that check how many
threads we want to create.
Inside, we'll put the pthread_create() function.
In param., we send the address of the array that will contain
our different threads.

As we simply create our thread, we can now call pthread_join()
function in the same for loop.

The result will be here 40000000, which is what we're expected.

Let's see what's going on, and print a message after the creation
of thread & after the pthread_join().
We'll notice interesting things :
Thread 0 has started, then it has finished execution.
Then, thread 1 starts execution and so on & so fourth.

Is that whenever when we finish the execution of a thread
do we get to start another one ?
That has to do with the way the for loop works here, for i
equals to zero we're creating the 1st thread and then we're
also joining it.
So, the 1st thread gets created, then the system waits for that
threads to finish its execution, & only then the 2nd thread can start
its execution.

So, that's NOT the way to create threads in a for loop.
We shouldn't both create & join in the same for loop because
we're gonna have sequential execution (one by one).

CORRECT WAY : 

We should create another for loop after we've created all the threads.
This for loop will be the same as the one for creating threads.
What's the result ? 
All threads are starting in parallel so it says that four
threads have started their execution at the same time, & then,
more or less the same time, they are being joined (they finished their execution).
But it doesn't mean that these threads finished in a specific order.
*/

#include <pthread.h>

int	mails = 0; // Var. represents the nb of mails.
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
	pthread_t	th[4];
	int			i;
	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < 4; i++)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
			return (1);
		printf("Thread %d has started\n", i);
	}
	for (i = 0; i < 4; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
			return (2);
		printf("Thread %d has finished execution\n", i);
	}
	pthread_mutex_destroy(&mutex);
	printf("Nb of mails : %d\n", mails);
	return (0);
}