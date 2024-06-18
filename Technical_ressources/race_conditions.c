
/*
RACE CONDITIONS : 

Removed everything in the function routine() in order to
change it & to encounter this race condition.

 Exemple :
 Imagine that we have a sort of mailbox with few listeners
 that increment the nb of mails.
 So, we have two listeners, represented by 2 threads & they
 listen for mails all across the globe.

In the routine funciton, we're gonna simulate adding
mails to the mailbox (incrementation).

When the threads finished their execution, we just want to print
out the values.
As we can see, we are putting our printf after thread function,
so we know that threads have finished their execution.

Result ? 
Mails is equal to 2, as we have 2 threads that both increment
this mails value. First thread is incrementing mails from 0 to 1,
& 2nd one from 1 to 2, as they share the same ressources.

What's happening if we increment it multiple times in the same thread ?
Let's do a loop in the routine function to increment mails.
Since each thread is going to increment this mails var. a
hundred times, we should get 100 times 2, the nb of threads.
Result ? We get 200 mails as we have 2 threads.

Let's increase the nb of mails (100 000), so we get the right
nb that is multiplied by 2 (nb of threads).

What's happening if we go further with 1 million ?
We're not getting 2 million as a result (less).
Why ? That is called a race condition.
What's actually happening behind the scene - to increment a var. :
- We read the value in mails
- We increment the value
- We write it back to the memory, so we write mails.

=> That's ok to do like this with one thread, but as soon
as we introduce multiple threads something strange could happen.
The column represents the execution of a thread.
The line represents the actual operation.
Example : 
				t1		t2
read mails		23		24
increment		23		24
write mails		24		25

- Thread 1 -
1. The thread 1 reads the value from mails 
2. He increments it, but that's not gonna change the value
from the var. it just increments locally in the cpu.
3. Then it writes the mails variable, the value becomes 24.

- Thread 2 -
1. When thread 1 finished, thread 2 is gonna start to read mails value.
2. Then it increments it, nothing is changing as the thread
increment it locally.
3. Then it writes it, and change the value of the mails var.

=> Each thread incremented the mail var. once. 

Another scenario :
If both of the threads are executing all 3 operations at the same
time, we have :
Example : 
				t1		t2
read mails		23		23
increment		23		
write mails		24		

- Thread 1 -
1. T1 reads mails so it's equal to 23.
2. T1 continues to execute itself, while T2 stoped.
So, it'll increment the mail value.
3. Then it'll write mails with the value of 24.

Example : T1 will still execute, & T2 is still on pause.
				t1		t2
read mails		24		23
increment		24		24
write mails		25		25

1. So it'll read 24 as we incremented the value before.
... (same steps)

- Thread 2 -
1. T2 reads also 23 as they execute at the same time.
=> For some reasons, thread2 just stops.
The operating system says take a break, we have to let
t1 do its thing.
2. After waiting all the iterations of T1, T2 starts again.
It'll not take the value modified by T1, but it's going to look
at what it has inside its cpu register (not inside memory !)
=> It says ok it's 23, but increment, this value is still
the value inside variable, which is 30 (T1 changed it).
3. We're gonna write 24, all of sudden we went from 30 to 24.
But this is an ISSUE, because it no longer guarantees that
we have correct results (if we do write operations in multiple
frames), that's what's called a race condition.

Example : T1 will still execute, & T2 is still on pause.
				t1		t2
read mails		29		23
increment		29		30
write mails		30		24

But why did it happen only at 1 million ?
Because if you have 11 iterations, you're gonna create the
thread & all these iterations are finished by the time you
actually create the second thread.
So you've nothing that interleaved.

*/

#include <pthread.h>

int	mails = 0; // Var. represents the nb of mails.

void	*routine()
{
	for (int i = 0; i < 100000, i++)
		mails++;
}

// Threads
int	main(int argc, char *argv[])
{
	pthread_t	t1, t2;
	if (pthread_create(&t1, NULL, &routine, NULL) != 0)
		return (1);
	if (pthread_create(&t2, NULL, &routine, NULL) != 0)
		return (1);
	if (pthread_join(t1, NULL) != 0)
		return (2);
	if (pthread_join(t2, NULL) != 0)
		return (2);
	printf("Nb of mails : %d\n", mails);
	return (0);
}