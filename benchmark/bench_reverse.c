#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "reverse.h"

#define BENCH_NUMBER 6
#define N 10000000
#define ALLOC_SIZE N*sizeof (uint32_t)

static double elapsed_seconds(struct timeval *start, struct timeval *end) {
    long seconds = end->tv_sec - start->tv_sec;
    long useconds  = end->tv_usec - start->tv_usec;
    double elapsed = seconds + useconds*1e-6;
    return elapsed;
}

struct benchcase {
    char *bench_name;
    void (*func)(unsigned *, const unsigned *, unsigned);
};

static const struct benchcase BENCHCASES[BENCH_NUMBER] = {
    {.bench_name="32b reverse with LOOKUP",     reverse_uint_lookup},
    {.bench_name="32b reverse with LOG",        reverse_uint_log},
    {.bench_name="32b reverse with LOG AVX256", reverse_uint_log_avx256},
    {.bench_name="32b reverse with 64b MUL 1",  reverse_uint_64bmul_1},
    {.bench_name="32b reverse with 64b MUL 2",  reverse_uint_64bmul_2},
    {.bench_name="32b reverse with no64b MUL",  reverse_uint_no64bmul}
};

int bench_reverse() {
    uint32_t *input_vector = (uint32_t *)aligned_alloc(32, ALLOC_SIZE);
    uint32_t *output_vector = (uint32_t *)aligned_alloc(32, ALLOC_SIZE);

    for (int i = 0; i < N; i++) {
        input_vector[i] = (uint32_t)random();
    }

    struct timeval start, end;
    double elapsed;

    for (int i = 0; i < BENCH_NUMBER; i++) {
        gettimeofday(&start, NULL);
        BENCHCASES[i].func(output_vector, input_vector, N);
        gettimeofday(&end, NULL);
        elapsed = elapsed_seconds(&start, &end);
        printf("--- BENCHMARK for %s\n", BENCHCASES[i].bench_name);
        printf("%f seconds elapsed => %.2f GB/s\n\n", elapsed, ALLOC_SIZE/elapsed/1e9);
    }

    free(input_vector);
    free(output_vector);

    exit(0);
}
