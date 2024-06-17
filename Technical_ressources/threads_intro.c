
/*
Include <pthread.h>

"-pthread" (added in the makefile ?)

One more thing we have to do extra besides just including
the file itself (header), is a link option that goes to the
command that compiles our source code.

Basis of creating a thread is that : 
When we create a thread it executes a function whatever
function you create.

So, 1st step : 
We're going to start by creating a simple routine that 
does something really generic. And then gonna try to execute
it using the thread creation.

To start creating threads we have to define a sort of place
where the api can store some information about the thread.
This place is a variable => The type is pthread_t

To initialize thread : 
1. We have to call the pthread_create() function.
	- First param : is the pointer to the t1 var.
	- Second param (the attributes) : are kind of customization
	of certains things. BUT, we can put NULL (it's a pointer) &
	 it's going to set it up itself.
	 - Third param : is the actual function that is going
	 to execute when it creates the thread. (passing function pointer)
	 - Fourth param : the argument that is going to be pass
	 to the routine.

One last thing we've to do, similar to how we do with processes,
we have to wait for that process (thread) to finish it's execution.
How do we have to do ?
=> We call another function : pthread_join();
It's like the wait but for threads !
Parameters : 
	- 1st param : the actual structure (t1), info needed for our threads.
	- 2nd param : pointer that gets the result from the trend. BUT since
	we don't return anything, we don't do anything special with the thread,
	we don't care so we pass NULL.

That's really ALL we need to do to create for creating a thread.
1. Create a structure with the informations concerning the thread.
2. Call pthread_create function();
3. Call pthread_join function();

RESULT OF THE TEST : 
The printf in the routine function is printed the message.

But how do i know that this is in parallel ?
We can try to create more threads ! How to do this ?
1. We just have to declare another pthread_t type var. (so t2 here)
2. Call pthread_create() function again & we execute the same function
(routine). So we call the same function but it's going to be from another thread.
3. Wait until it's finished executing t2, as the first one. 

EXECUTION : 
We see, that the routine function will print on the screen twice.

But, how do i know that things are executed in parallel ??
What we can do is to print 2 messages inside a function.
To do this => We're gonna call the sleep() function, & then
print another message.
So, if this function would be executing twice one after the other,
we would expect that first we see "Tests" from threads on the console,
and then we'll see "ending thread".

BUT, when we execute it : we see that the first printf is printed
twice and then the second printf, printed twice after.

WHAT DOES IT MEAN ?
That both threads executed that printf statement, then it got
to the sleep of 3 seconds. So, both threads kind of pause their execution.
And then printed again but the 2nd printf (ending almost at the same time).

WARNING : 
We've to check error function call because, here we're assuming that
we can create these threads but in linux envt we can actually suppress
people from creating threads or simply just not have enough ressources
to create those threads.
*/

void	*routine()
{
	printf("Test from threads\n");
	sleep(3); // We wait a while,
	printf("Ending thread\n");
}

int	main(int argc, char *argv[])
{
	pthread_t	t1, t2; // Struct. to store some info about the thread.
	if (pthread_create(&t1, NULL, &routine, NULL) != 0) // Since we don't have arg. here we pass null.
		return (1); // Error code.
	// We are going to check if this pthread create returns an int (zero or !=).
	if (pthread_create(&t2, NULL, &routine, NULL) != 0)
		return (2);
	if (pthread_join(t1, NULL) != 0)
		return (3);
	if (pthread_join(t2, NULL) != 0)
		return (4);
	return (0);
}