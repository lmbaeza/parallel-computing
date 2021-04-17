// gcc -fopenmp -lm -D_DEFAULT_SOURCE -o posix_WITH_pad posix_WITH_pad.c && ./posix_WITH_pad
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define ITERATIONS 1e09
#define THREADS 4
#define PAD 8

double piTotal[THREADS*PAD];

void *calculatePi(void *arg) {
    int startIter, endIter, ID = *(int *)arg;

    startIter = (ITERATIONS/THREADS) * ID;
    endIter = ((ITERATIONS/THREADS) * (ID+1));

    int j = ID*PAD;
    piTotal[j] = 0.0;

    do {
        piTotal[j] = piTotal[j] + (double)(4.0 / ((startIter*2)+1));
        startIter++;
        piTotal[j] = piTotal[j] - (double)(4.0 / ((startIter*2)+1));
        startIter++;
    } while(startIter < endIter);

    return 0;
}

int main() {
    
    int threadId[THREADS], i, *retval;
    pthread_t thread[THREADS];
    struct timeval tval_before, tval_after, tval_result;

    gettimeofday(&tval_before, NULL);
    
    for(i = 0; i < THREADS; i++){
        threadId[i] = i;
        pthread_create(&thread[i], NULL, (void *)calculatePi, &threadId[i]);
    }

    for(i = 0; i < THREADS; i++){
        pthread_join(thread[i], (void **)&retval);
    }

    for(i = 1; i < THREADS; i++){
        // *piTotal = *piTotal + *(piTotal + (i*PAD));
        piTotal[0] = piTotal[0] + piTotal[i*PAD];
    }

    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    
    printf("\npi: %2.12f \n", piTotal[0]);

    return 0;
}