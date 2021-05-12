#pragma once

void reverse_uint_naive(unsigned int *output, const unsigned int *input, unsigned int size);
void reverse_uint_lookup(unsigned int *output, const unsigned int *input, unsigned int size);
void reverse_uint_64bmul_1(unsigned int *output, const unsigned int *input, unsigned int size);
void reverse_uint_64bmul_2(unsigned int *output, const unsigned int *input, unsigned int size);
void reverse_uint_no64bmul(unsigned int *output, const unsigned int *input, unsigned int size);
void reverse_uint_log(unsigned int *output, const unsigned int *input, unsigned int size);
void reverse_uint_log_avx256(unsigned int *output, const unsigned int *input, unsigned int size);
