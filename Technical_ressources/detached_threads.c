
/*
DETACHED THREADS

We can detach threads from the main thread. 
There is a function we can call for each thread, once we
created a thread we can use pthread_detach(), in the for loop
where we create our threads.

It takes the actual thread that we want to detach.
If we detach all of the threads, what's gonna happen ?
We get error messages as it failed to join the thread.
=> it didn't go to the execution (routine() function).

So, a detached thread is no longer joinable, we cannot call
the pthread_join on it so that means that we cannot wait for it
to finish its execution before exiting the prog.

Another thing about detached thread, is that they clear their
own ressources so, before with pthread_join, we have to call
it on every joinable thread that we create (to clear ressources
that we're using).

But, with a detached thread we no longer have to do that &
if we call pthread_join, we do have an error.

But why the thread is not even finishing is execution ?
It's because the actual process, as a whole, finished its
execution so the main thread, that created threads detached
them, it failed to join them.
All the threads were stuck at this sleep(1) in routine () function,
& then the main thread called return zero.
=> We can use the pthread_exit() function, that will wait
for those threads that are part of the same process (main thread).

Detached thread is usually used when you have a long running
process that we want to start inside the main thread, but
we don't want to let the main thread keep on running.

So, in our example we don't need to join them because they
are already detached, so we can therefore exit & we can let
those threads finish their execution on their own.
As we can see, the process doesn't finish its execution,
the process itself, until all of the detached threads have
finished their execution.

ANOTHER WAY we can set a thread to be detached : 
Because this is not really setting the thread to be detached,
it's just creating the thread in a non-detached state &
then it's setting/changing it to be detached.

How can we create it as detached thread without having
to change it after we've created it.
=> To do this we're going to make use of the second param.
inside the pthread_create !

First, we have to create this attribute by creating another
variable called pthread_attr_t detachedThread.
Then, we call the pthread_attr_init() function, & send
inside the reference to the variable we created before.
Then, we're going to call pthread_attr_setdetachstate() function.
Don't forget ! We have to destroy these objects thanks to
pthread_attr_destroy() function by passing the reference to
the detachedThread.


So, we can finally use the 2nd parameter of the pthread_create()
function. We can now clarify few things concerning the creation
of threads. 
The reason of using the pthread_attr_t is because that there
might be a case where the thread would finish its execution
right in between calling pthread_create & pthread_detach.
So, the ressources wouldn't be freed automatically because
it finished execution and wasn't joined & it wasn't in a
detached state.
In this way, we're making sure that the threads are always
created with the detached state set.

In this pthread_attr object you can actually set a lot more
things than just the detached state.

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define THREAD_NUM 2

// FIRST VERSION - DETACHED THREADS

void* routine(void* args) {
    sleep(1);
    printf("Finished execution\n");
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], &detachedThread, &routine, NULL) != 0) {
            perror("Failed to create thread");
        }
        pthread_detach(th[i]);
    }
    // for (i = 0; i < THREAD_NUM; i++) {
    //     if (pthread_join(th[i], NULL) != 0) {
    //         perror("Failed to join thread");
    //     }
    // }
    pthread_exit(0);
}

// SECOND VERSION

void* routine(void* args) {
    sleep(1);
    printf("Finished execution\n");
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    pthread_attr_t detachedThread;
    pthread_attr_init(&detachedThread);
    pthread_attr_setdetachstate(&detachedThread, PTHREAD_CREATE_DETACHED);
    
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], &detachedThread, &routine, NULL) != 0) {
            perror("Failed to create thread");
        }
        // pthread_detach(th[i]);
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    pthread_attr_destroy(&detachedThread);
    pthread_exit(0);
}