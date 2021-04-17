// gcc -fopenmp -lm -D_DEFAULT_SOURCE -o OpenMP_using_Reduction OpenMP_using_Reduction.c && ./OpenMP_using_Reduction
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>
 
#define MAX_TERMS 2e09
#define NUMTHREADS 8
 
int main() {
    int thread_count = NUMTHREADS;
    double result = 0;

    struct timeval tval_before, tval_after, tval_result;

    gettimeofday(&tval_before, NULL);
    
    #pragma omp parallel for num_threads(thread_count) reduction(+:result)
    for(int i = 0; i < MAX_TERMS; ++i) {
        result += 4.0 * (i % 2 == 0 ? 1: -1) / (2.0 * i + 1);
    }

    gettimeofday(&tval_after, NULL);
 
    timersub(&tval_after, &tval_before, &tval_result);
 
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

    printf("Result: %.16lf\n", result);

    return 0;
}