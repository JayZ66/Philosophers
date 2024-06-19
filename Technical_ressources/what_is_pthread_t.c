
/*
WHAT IS PTHREAD_T ??

What's actually the pthread_t & clarify things about the
is of a thread.

We get the ID number of our threads, & that's what pthread_create
& pthread_join functions are using to talk to the same thread.
So, pthread_create sets the numbers (in the array) and
pthread_join identifies each one of those threads using
exactly these numbers.

Another thing to do if we want these nb inside of the thread itself :
Using pthread_self() function & doesn't take any param.
but just give the result to a pthread_t variable.

WE SHOULDN'T DO THAT !
Because on the specification of the pthread api, they said
that pthread_t should be treated as an opaque data type.
Meaning that it's not always guaranteed that it is goin to
be unsigned long type.
So, we shouldn't work with the values themselves inside of it.
We can't directly print (for ex.) the value of the pthread_t var.

We also have the getpid() function to get the actual process id.
Linux has a similar function called gettid() that gets the thread
id. To call this is a bit more complex (see below !).

The pb is that the SYS_gettid, even though it stands for thread
id, it's linux specific so if we launch it like the example,
we'll get differents values !
They are different because namely the pthread_t is an id
that is managed by the pthread api itself. 

Software hierachy point of view : 
On top, we have our program.
Below it we have the pthread api. (pthread_t is manager at this level)
Below we have the operating system. (gettid is the actual internal thread id).

=> That's why we can have different results, it's because they are
different in the linux system.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/syscall.h>

#define THREAD_NUM 2

void* routine(void* args) {
    pthread_t th = pthread_self();
    printf("%ul\n", th);
    printf("%d\n", (pid_t) syscall(SYS_gettid));
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("Failed to create thread");
        }
        // printf("%ul\n", th[i]);
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    return 0;
}