
/*
EXAMPLE THREADS
Will create 2 threads & want to sum up all the numbers
in this primes array.

How to do it ?
We'll split the array, the first half gonna be sent to the
1st thread & the second hafl to the 2nd thread.
Then, we're gonna get back a value, a sum, of all those
numbers.
Then, in the main thread, we're gonna sum all the half sums
together and get the final result.

- Steps -
1. Change the number of threads (only 2 threads)
Still keep the malloc because we're gonna pass some values to it.
But we need to define what will be the value of the *a var.
We want to pass the start index from which we want to sum
half of the array.
=> We pass to the 1st thread the index to the first element.
=> To the 2nd thread, we want to pass the index to the sixth
element. So we give i * 5
In order to get the 1st half & then the 2nd with the rigth nb.

2. Let's manage the routine function.
We have to create a sum, so we do a for loop.
Now we have the sum, but how do we send it back ?
Two options : 
- We can malloc another place in memory & return that.
- Just return the param. (place in memory that we have already
dynamically allocated).
Best way of handling dynamically allocated memory : 
Passing it to the thread & make sure that the thread returns
it back so that we can then de-allocated in the main function.

3. So, we declare a new var. in the main function, but in the
second loop. We create int *r var, then we pass the reference
to a pointer in the pthread_join() function.
Nothing else, because this pointer is going to be set to
whatever we return in the routine() function.
We know that in our r variable, we have inside a pointer to
the sum itself, so we can create another var.

4. Creating a new var. in the main function : int global_sum
Then we give it as value, the sum of each thread (+=), to do
that we have to dereference the r variable.

5. Don't forget to free the memory that we've allocated !
At the end of main function we have to free(r) because we
allocated it in the beginning of the main function (1st loop).
How ? 
We malloc the int *a in the 1st loop, then passed through the
pthread_create, pass in routine parameters function, & then
return back, inside r and gave to the pthread_join function
to get the sum result.
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void* routine(void* arg) {
    int index = *(int*)arg;
    int sum = 0;
    for (int j = 0; j < 5; j++) {
        sum += primes[index + j];
    }
    printf("Local sum: %d\n", sum);
    *(int*)arg = sum;
    return arg;
}

int main(int argc, char* argv[]) {
    pthread_t th[2];
    int i;
    for (i = 0; i < 2; i++) {
        int* a = malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread");
        }
    }
    int globalSum = 0;
    for (i = 0; i < 2; i++) {
        int* r;
        if (pthread_join(th[i], (void**) &r) != 0) // Necessary to cast our var. !
		{
            perror("Failed to join thread");
        }
        globalSum += *r;
        free(r);
    }
    printf("Global sum: %d\n", globalSum);
    return 0;
}
