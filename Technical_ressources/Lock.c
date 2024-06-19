
/*
LOCK

Subject : Take a look at the pthread_mutex_lock.

In the routine function we want to lock a single mutex by
all the four threads that we have created & print a message.
So, you call the pthread_mutex_lock() function on our mutex
& also call the pthread_mutex_unlock() function after we are
done !

Explanation : All four threads are gonna start executing,
the same exact function but only one of them are gonna
be able to lock that mutex.
The rest gonna have to wait & then only after that, the
single thread that i got the lock, unlock that mutex & then
another thread continue.

What's a trylock ?
You're, here trying to lock the mutex (so not guaranteed that it is
locked) + always check the return value !

*/

int	main(int argc, char *argv[])
{

	return 0;
}