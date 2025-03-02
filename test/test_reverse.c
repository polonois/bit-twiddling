#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reverse.h"

#define TEST_NUMBER 6
#define N 100000
#define ALLOC_SIZE N*sizeof (uint32_t)

struct testcase {
    char *test_name;
    void (*func)(unsigned *, const unsigned *, unsigned);
};

static const struct testcase TESTCASES[TEST_NUMBER] = {
    {.test_name="32b reverse with LOOKUP",     reverse_uint_lookup},
    {.test_name="32b reverse with LOG",        reverse_uint_log},
    {.test_name="32b reverse with LOG AVX256", reverse_uint_log_avx256},
    {.test_name="32b reverse with 64b MUL 1",  reverse_uint_64bmul_1},
    {.test_name="32b reverse with 64b MUL 2",  reverse_uint_64bmul_2},
    {.test_name="32b reverse with no64b MUL",  reverse_uint_no64bmul}
};

void test_reverse() {
    // Initialize all the test vectors
    uint32_t *input_vector = (uint32_t *)aligned_alloc(32, ALLOC_SIZE);
    uint32_t *output_vector = (uint32_t *)aligned_alloc(32, ALLOC_SIZE);
    uint32_t *ref_output_vector = (uint32_t *)aligned_alloc(32, ALLOC_SIZE);

    for (int i = 0; i < N; i++) {
        input_vector[i] = (uint32_t)random();
    }

    // Reference vector to be tested against
    reverse_uint_naive(ref_output_vector, input_vector, N);

    for (int i = 0; i < TEST_NUMBER; i++) {
        TESTCASES[i].func(output_vector, input_vector, N);

        int res = memcmp(ref_output_vector, output_vector, ALLOC_SIZE);
        printf("%s is %s\n", TESTCASES[i].test_name, res ? "INCORRECT" : "correct");

        memset(output_vector, 0, ALLOC_SIZE);
    }

    free(input_vector);
    free(output_vector);
    free(ref_output_vector);
}
