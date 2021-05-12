#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reverse.h"

#define N 100000
#define ALLOC_SIZE N*sizeof (unsigned int)

void test_reverse() {
    unsigned int *input_vector = (unsigned int *)aligned_alloc(32, ALLOC_SIZE);
    unsigned int *output_vector = (unsigned int *)aligned_alloc(32, ALLOC_SIZE);
    unsigned int *ref_output_vector = (unsigned int *)aligned_alloc(32, ALLOC_SIZE);

    for (int i = 0; i < N; i++) {
        input_vector[i] = (unsigned int)random();
    }

    reverse_uint_naive(ref_output_vector, input_vector, N);

    reverse_uint_lookup(output_vector, input_vector, N);
    if (memcmp(ref_output_vector, output_vector, ALLOC_SIZE) == 0) {
        printf("Reverse uint lookup is correct\n");
    } else {
        printf("Reverse uint lookup is INCORRECT\n");
    }
    memset(output_vector, 0, ALLOC_SIZE);

    reverse_uint_log(output_vector, input_vector, N);
    if (memcmp(ref_output_vector, output_vector, ALLOC_SIZE) == 0) {
        printf("Reverse uint log is correct\n");
    } else {
        printf("Reverse uint log is INCORRECT\n");
    }
    memset(output_vector, 0, ALLOC_SIZE);

    reverse_uint_log_avx256(output_vector, input_vector, N);
    if (memcmp(ref_output_vector, output_vector, ALLOC_SIZE) == 0) {
        printf("Reverse uint log avx256 is correct\n");
    } else {
        printf("Reverse uint log avx256 is INCORRECT\n");
    }
    memset(output_vector, 0, ALLOC_SIZE);
    
    reverse_uint_64bmul_1(output_vector, input_vector, N);
    if (memcmp(ref_output_vector, output_vector, ALLOC_SIZE) == 0) {
        printf("Reverse uint 64bmul_1 is correct\n");
    } else {
        printf("Reverse uint 64bmul_1 is INCORRECT\n");
    }
    memset(output_vector, 0, ALLOC_SIZE);

    reverse_uint_64bmul_2(output_vector, input_vector, N);
    if (memcmp(ref_output_vector, output_vector, ALLOC_SIZE) == 0) {
        printf("Reverse uint 64bmul_2 is correct\n");
    } else {
        printf("Reverse uint 64bmul_2 is INCORRECT\n");
    }
    memset(output_vector, 0, ALLOC_SIZE);

    reverse_uint_no64bmul(output_vector, input_vector, N);
    if (memcmp(ref_output_vector, output_vector, ALLOC_SIZE) == 0) {
        printf("Reverse uint no64bmul is correct\n");
    } else {
        printf("Reverse uint no64bmul is INCORRECT\n");
    }
    memset(output_vector, 0, ALLOC_SIZE);

    free(input_vector);
    free(output_vector);
    free(ref_output_vector);
}
