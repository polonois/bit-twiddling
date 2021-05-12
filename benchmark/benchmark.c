#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "reverse.h"

#define N 10000000
#define ALLOC_SIZE N*sizeof (unsigned int)

static double elapsed_seconds(struct timeval *start, struct timeval *end) {
    long seconds = end->tv_sec - start->tv_sec;
    long useconds  = end->tv_usec - start->tv_usec;
    double elapsed = seconds + useconds*1e-6;
    return elapsed;
}

int main() {
    unsigned int *input_vector = (unsigned int *)aligned_alloc(32, ALLOC_SIZE);
    unsigned int *output_vector = (unsigned int *)aligned_alloc(32, ALLOC_SIZE);

    for (int i = 0; i < N; i++) {
        input_vector[i] = (unsigned int)random();
    }

    struct timeval start, end;
    double elapsed;

    gettimeofday(&start, NULL);
    reverse_uint_naive(output_vector, input_vector, N);
    gettimeofday(&end, NULL);
    elapsed = elapsed_seconds(&start, &end);
    printf("Naive implementation\n");
    printf("%f seconds elapsed => %.2f GB/s\n", elapsed, ALLOC_SIZE/elapsed/1e9);

    gettimeofday(&start, NULL);
    reverse_uint_log(output_vector, input_vector, N);
    gettimeofday(&end, NULL);
    elapsed = elapsed_seconds(&start, &end);
    printf("Log implementation\n");
    printf("%f seconds elapsed => %.2f GB/s\n", elapsed, ALLOC_SIZE/elapsed/1e9);

    gettimeofday(&start, NULL);
    reverse_uint_log_avx256(output_vector, input_vector, N);
    gettimeofday(&end, NULL);
    elapsed = elapsed_seconds(&start, &end);
    printf("Log avx256 implementation\n");
    printf("%f seconds elapsed => %.2f GB/s\n", elapsed, ALLOC_SIZE/elapsed/1e9);
    
    gettimeofday(&start, NULL);
    reverse_uint_64bmul_1(output_vector, input_vector, N);
    gettimeofday(&end, NULL);
    elapsed = elapsed_seconds(&start, &end);
    printf("3 OPs 64MUL implementation\n");
    printf("%f seconds elapsed => %.2f GB/s\n", elapsed, ALLOC_SIZE/elapsed/1e9);

    gettimeofday(&start, NULL);
    reverse_uint_64bmul_2(output_vector, input_vector, N);
    gettimeofday(&end, NULL);
    elapsed = elapsed_seconds(&start, &end);
    printf("4 OPs 64MUL implementation\n");
    printf("%f seconds elapsed => %.2f GB/s\n", elapsed, ALLOC_SIZE/elapsed/1e9);

    gettimeofday(&start, NULL);
    reverse_uint_no64bmul(output_vector, input_vector, N);
    gettimeofday(&end, NULL);
    elapsed = elapsed_seconds(&start, &end);
    printf("7 OPs no 64MUL implementation\n");
    printf("%f seconds elapsed => %.2f GB/s\n", elapsed, ALLOC_SIZE/elapsed/1e9);

    free(input_vector);
    free(output_vector);
}
