#ifdef __STDC_NO_THREADS__
    #error "This program requires C11 <threads.h> to build"
#endif
#include <threads.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int index;
    int result;
} FibData;

int Fibonacci(void *args) {
    FibData *data = (FibData *)args;
    // each thread will work to calculate the sequence at specific term
    int idx = data->index;
    if (idx == 0) {
        data->result = 0;
    } else if (idx == 1) {
        data->result = 1;
    } else {
        int num1 = 0, num2 = 1, num3;
        // i = 2; because we hardcoded 0th and 1st term in fib in if & elif block
        for (int i = 2; i <= idx; i++) {
            num3 = num1 + num2;
            num1 = num2;
            num2 = num3;
        }
        data->result = num2;
    }
    return 0;
}

int main() {
    int terms;
    printf("How many terms in the Fibonacci Sequence?\n");
    scanf("%d", &terms);
    if (terms <= 0) {
        printf("Can't have less than 0 terms\n");
        return 1;
    }

    thrd_t *threads = malloc(terms * sizeof(thrd_t));
    FibData *data = malloc(terms * sizeof(FibData));
    if (!threads || !data) {
        printf("Malloc failed\n");
        free(threads);
        free(data);
        return 1;
    }

    for (int i = 0; i < terms; ++i) {
        data[i].index = i;
        // thrd_create needs to be called anyways before the if block can see if it's successful or not.
        // so line # not only checks if the thread creation is successful, but create the thread so it can check.
        if (thrd_create(&threads[i], Fibonacci, &data[i]) != thrd_success) {
            printf("Thread creation failed\n");
            free(threads);
            free(data);
            return 1;
        }
    }

    for (int i = 0; i < terms; ++i) {
        thrd_join(threads[i], NULL);
    }

    printf("Fibonacci: ");
    for (int i = 0; i < terms; ++i) {
        printf("%d ", data[i].result);
    }
    printf("\n");
    free(threads);
    free(data);
    return 0;
}