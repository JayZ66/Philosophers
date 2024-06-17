
/*
THREADS VS PROCESSES : 

It's kind of the same thing, as we execute the code
at the same time. 
With threads we have a function we execute with.
With processes we just execute the code from fork onwards.

What's the difference ??
To show the difference, there gonna be two main files :
1. First one, is gonna be implemented using threads.
2. Second one, is gonna be implemented using processes.

STEPS : 
1. We're going to create 2 processes & 2 threads.
	If i just create one thread, i'll have just one line printed.

Let's say we want to print the process id's first => We sould get
2 different nbs.
But, what happens if we want to do the same with threads ?
=> We get two of the same id's !

WHY ? What are the differences ?
1. Because processes can contain multiple threads.
	So we can have, multiple threads inside a single process but
	not the other way around.
	So, sort of processes encompass the envt of threads, & we can
	have multiple of them that is one difference.
2. Regarding the adress space
	When we were working with PROCESSES, we were forking, so it means
	that we are duplicating all the var. into the child p.
	So, we get 2 different var. one on each process & we can modify
	them individually.
	BUT, with THREADS, there is a big diff. where we have all the var.
	in the same place.
	All threads can access all variables, and they all have
	a common set of variables, so a common address space.

ILLUSTRATION : to illustrate it we can do a simple add.
Processes : 
	We want to make sure that we're actually incrementing this var.
	and then we're printing it on both processes.
	First, we're gonna sleep for 2 sec., & that sleep is gonna occur
	for both processes.
	Result ? An interesting thing will happen :
	- For one process, the value of x was 2
	- For the 2nd one, the value of x was 3
So, in only one of the process the x var. got incremented.
On the other one, it stayed the same (initialized);

Threads :
	We're gonna create another function, & change the second thread
	to call the new function.
	- So, in the first one we'll incremente the var. x, then
	sleep(2) & write the value of x.
	- In the second one, we're not gonna increment x.
	Result ? 
	We notice that the value of x is 3 on both threads.
	Even though we have only incremented the x var. in the thread1,
	thread 2 went along and slept for 2 sec. (like the 1st one)
	So, we made sure that both printf was executed after the incrementation in thread 1.

CONCLUSION : 
It shows that with the processes, each process as its own envt, with its own
variables, etc. So, when we are doing something, only the process we are in
is impacted.
One the other way, threads are sharing the same envt, so the same variables
and each modification will have an impact on each thread.

=> So, threads are sharing memories, interconnection. (one of the biggest diff.)
But there comes a trade-off because since they share memory,
there could be issues when it comes to certain threads modifying
the same var.

=> All the system ress. are shared between all the threads.
Others ?
- All the threads are sharing the file handlers, whereas with processes
all the file handlers are duplicated.
*/

int	x = 2;

void	*routine2()
{
	sleep(2);
	printf("Value of x : %d\n", x);
}

void	*routine()
{
	x++;
	sleep(2);
	printf("Value of x : %d\n", x);
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
	return (0);
}

// Processes
int	main(int argc, char *argv[])
{
	int	x = 2;
	int pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0) // We're in the child p.
		x++;
	sleep(2); // Since we waited we are sure that the incremention occured.
	printf("Value of x : %d\n", x);
	// printf("Process id %d\n", getpid());
	if (pid != 0) // We wait for the child p. to finish its execution.
		wait(NULL);
	return (0);
}