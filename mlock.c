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
    int thread_id = *(int *)args;
    // thread will continuously work to decrement
    while (1) {
        // lock access to this thing
        mtx_lock(&mutex);
        // if shared_int reaches or passes 0, end
        if (shared_int <= 0) {
            mtx_unlock(&mutex);
            break;
        }
        --shared_int;
        printf("thread %d decremented shared_int: %d\n", thread_id, shared_int);
        mtx_unlock(&mutex);
        thrd_yield;
    }
    return 0;
}

int main() {
    thrd_t threads[number_of_threads];
    int thread_ids[number_of_threads];

    // Initialize Mutex
    if (mtx_init(&mutex, mtx_plain) != thrd_success) {
        printf("Failed to initialize mutex\n");
        return 1;
    }

    // Initialize Threads
    for (int i = 0; i < number_of_threads; ++i) {
        thread_ids[i] = i+1;
        // if contidion creates the thread in order to check the success condition
        if (thrd_create(&threads[i], countdown, &thread_ids[i]) != thrd_success) {
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
    return 0;
}