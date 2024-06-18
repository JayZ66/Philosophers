
/*
HOW TO PASS ARGUMENTS TO THREADS IN C ?

We're going to see how to pass certain unique values
to each thread.

So, we want 10 threads & we want that each of them just print
one (unique) prime from the array.

The problem : 
Create 10 threads, each taking a unique prime from the primes
array & print it on the screen.

Who can we do that ? 
1. Create the 10 threads (for loop).
2. We create our routine function, but this time we're going to pass
in a void pointer arg.
=> It's what we're gonna be able to pass in to the routine.
So, we cann put a printf, inside, to printf our primes value
with the rand() function. 
But how can we print a unique value from primes[] per thread ?

So, the fourth arg. of pthread_create is the arg. of our routine() function.
What this does ?
This var. is called JustARG, his passes a var., a position in
memory, to our routine() function.
So, if we pass something in the fourth param., it's going to
get in the routine() param. & we can use it.

We create an int variable called index, in routine function,
which the value will be the dereference arg.
To do it, we have to cast it first + dereference it at the same time.

Then, we want to pass it to the pthread_create(), as we cannot
ust pass i, that's not a pointer, we can pass the ref.

Result ? 
We printed primes on the screen but not each one of them.

Why ?
The issue here is that we're passing memory addresses instead
of just values. Here is the multi-threated nature of
how the i (of for loop main) does change irrelevant of what's
executed in the routine function.

We see here that if we add a sleep(1) at the beginning of the
routine() function, we get all values at 0.
Because if we print our index we see that it's always
equal to 10.
So why ? 
We have here 10 pthread_create calls so we have 10 tries that
get created, but they don't really start doing anything because
they sleep for one second & that one second all of them get
created.
So, it goes then to the second loop, after created the threads.
The i is different, and because of that we get whenever they
actually start executing we get i to be 10.

Which solution ?
Is to simply dynamically allocate some memory for this i that
we want to pass for each thread.

So, in the first loop (main) we create a int *a var. & we
allocate it memory. Then, we initialize it with the value of i.
So, at every allocation we actually allocate a separate memory
for every thread (4 bytes). & then pass the a var. to the function.
To free it, we have to do it in the routine function to no break
the thread !

*/

int	primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void	*routine(void *arg) // This arg. is the SAME as the 4 param. of phtread_create() !!
{
	int index = *(int*)arg;
	printf("%d ", primes[index]);
	free(arg);
}

int	main(int argc, char *argv[])
{
	pthread_t	th[10];

	for (int i = 0; i < 10; i++)
	{
		int *a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &routine, a) != 0)
		return 1;
	}
	for (int i = 0; i < 10; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
			return 2;
	}
	return (0);
}