#pragma once

#include <stdint.h>

void reverse_uint_naive(uint32_t *output, const uint32_t *input, uint32_t size);
void reverse_uint_lookup(uint32_t *output, const uint32_t *input, uint32_t size);
void reverse_uint_64bmul_1(uint32_t *output, const uint32_t *input, uint32_t size);
void reverse_uint_64bmul_2(uint32_t *output, const uint32_t *input, uint32_t size);
void reverse_uint_no64bmul(uint32_t *output, const uint32_t *input, uint32_t size);
void reverse_uint_log(uint32_t *output, const uint32_t *input, uint32_t size);
void reverse_uint_log_avx256(uint32_t *output, const uint32_t *input, uint32_t size);
