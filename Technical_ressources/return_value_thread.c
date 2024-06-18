
/*
GET RETURN VALUE FROM A THREAD

We saw how to create threads & to make them do something,
but we haven't looked how to get a result from them.

How can we get the return value from a thread into our
main function ?
First, we create a thread & then wait for the end of its
execution (with join).
So here, we'll have a function that'll roll a dice and return
the results in the main function (to print it).

How do we get the value from this thread function to the
main function ? 
It has to do with this second parameter of pthread_join();
In his parameters, it takes a double pointer to void.
It's a double pointer to void, because in the roll_dice()
function we defined it with a void * as the return type.
It's what we'll use to get the result of our function.

pthread_join() will take a reference to a pointer, & it
going to set the value from within it to whatever we have
sent from here. 

So, in the main we can create an int *var, then we pass the
reference of res to pthread_join().

As its a void pointer, we can send it everywhere BUT need
to cast it !

In the function roll_dice() we need to return a void * & not
an int.
So, we can return a reference to our value, so this will give
us an int pointer which can be casted to void pointer.

We have a SEGFAULT, why ? 
We return a reference to the value that is local to this function.
So, we can't really do that with functions, the local var.
will be deallocated because it's on the stack.

We will, instead of just casting the reference to this var.
to void pointer, create another result.
It's a var. which is an int *result;
We'll allocate memory with malloc with a size of int.
*result = value; => the value of the result var. is value.

So, our *result var. will be the same as the *res var in the main.
as we allocate memory for this variable, we'll have to deallocate it,
but inside our main.
*/

#include <pthread.h>
#include <time.h>

void	*roll_dice()
{
	int	value = (rand() % 6) + 1; // To create a value between 1 to 6.
	int	*result = malloc(sizeof(int));
	*result = value;
	printf("%d\n", value);
	return ((void *)result);
}

// Threads
int	main(int argc, char *argv[])
{
	int	*res;
	srand(time(NULL));
	pthread_t	th;

	if (pthread_create(&th, NULL, &roll_dice, NULL) != 0)
		return 1;
	if (pthread_join(th, (void **) &res) != 0)
		return 2;
	printf("Result: %d\n", *res); // We send the *res to get the value of this pointer.
	free(res);
	return (0);
}