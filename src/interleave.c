#include <limits.h>

void interleave_naive(unsigned int *output, unsigned short *input_1, unsigned short *input_2, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        unsigned short in1 = input_1[i];
        unsigned short in2 = input_2[i];
        unsigned int result = 0;

        for (int j = 0; j < sizeof(in1) * CHAR_BIT; j++) {
            result |= (in1 & 1U << j) << j | (in2 & 1U << j) << (j+1);
        }
    }
}

