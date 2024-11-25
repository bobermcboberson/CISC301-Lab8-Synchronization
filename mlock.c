#ifdef __STDC_NO_THREADS__
    #error "This program requires C11 <threads.h> to build"
#endif
#include <threads.h>
#include <stdio.h>

// Global Variable
int shared_int = 100;
// Number of Threads working
int number_of_threads = 5;
// Mutex lock type
mtx_t mutex;

// Countdown function
int countdown(void *args) {
    // thread will continuously work to decrement
    while (1) {
        // lock access to this thing
        mtx_lock(&mutex);
        // if conditon will see if we get to 0
        if (shared_int > 0) {
            --shared_int;
            printf("Thread %p decremented, new value: %d\n", args, shared_int);
            // unlock access to this thing
            mtx_unlock(&mutex);
        } else {
        // if the int <= 0, we unlock the mutex on the variable and break out of the loop
        // this will stop our while(1) loop
        // unlock access to the thing
            mtx_unlock(&mutex);
            break;
        }
    }
    return 0;
}

int main() {
    thrd_t threads[number_of_threads];
    long thread_ids[number_of_threads];

    // Initialize Mutex
    if (mtx_init(&mutex, mtx_plain) != thrd_success) {
        printf("Failed to initialize mutex\n");
        return 1;
    }

    // Initialize Threads
    for (int i = 0; i < number_of_threads; ++i) {
        thread_ids[i] = i + 1;
        // if contidion creates the thread in order to check the success condition
        if (thrd_create(&thread_ids[i], countdown, &thread_ids[i]) != thrd_success) {
            printf("failed to create thread %d\n", i + 1);
        }
    }

    // Join threads back
    for (int i = 0; i < number_of_threads; ++i) {
        thrd_join(threads[i], NULL);
    }

    // Kill mutex
    mtx_destroy(&mutex);

    printf("Final value of shared variable: %d\n", shared_int);
}