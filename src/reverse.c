#include <immintrin.h>
#include <limits.h>
#include <stdint.h>

void reverse_uint_naive(uint32_t *output, const uint32_t *input, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        uint32_t in = input[i];
        uint32_t result = in;
        int s = sizeof(uint32_t)*CHAR_BIT - 1;

        for (in >>= 1; in; in >>=1) {
            result <<= 1;
            result |= in & 1;
            s--;
        }
        result <<= s;
        output[i] = result;
    }
}

static const unsigned char bit_reverse_table256[256] =
{
#   define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
#   define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
#   define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
    R6(0), R6(2), R6(1), R6(3)
};
void reverse_uint_lookup(uint32_t *output, const uint32_t *input, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        unsigned char *in = (unsigned char *)&input[i];
        unsigned char *out = (unsigned char *)&output[i];
        out[3] = bit_reverse_table256[in[0]];
        out[2] = bit_reverse_table256[in[1]];
        out[1] = bit_reverse_table256[in[2]];
        out[0] = bit_reverse_table256[in[3]];
    }
}

unsigned char reverse_uchar_64mul_1(unsigned char input) {
    return (input * 0x0202020202ULL & 0x010884422010ULL) % 1023;
}
void reverse_uint_64bmul_1(uint32_t *output, const uint32_t *input, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        unsigned char *in = (unsigned char *)&input[i];
        unsigned char *out = (unsigned char *)&output[i];

        out[0] = reverse_uchar_64mul_1(in[3]);
        out[1] = reverse_uchar_64mul_1(in[2]);
        out[2] = reverse_uchar_64mul_1(in[1]);
        out[3] = reverse_uchar_64mul_1(in[0]);
    }
}

unsigned char reverse_uchar_64mul_2(unsigned char input) {
    return ((input * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
}
void reverse_uint_64bmul_2(uint32_t *output, const uint32_t *input, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        unsigned char *in = (unsigned char *)&input[i];
        unsigned char *out = (unsigned char *)&output[i];

        out[0] = reverse_uchar_64mul_2(in[3]);
        out[1] = reverse_uchar_64mul_2(in[2]);
        out[2] = reverse_uchar_64mul_2(in[1]);
        out[3] = reverse_uchar_64mul_2(in[0]);
    }
}

unsigned char reverse_uchar_no64bmul(unsigned char input) {
    return ((input * 0x0802LU & 0x22110LU) | (input * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
}
void reverse_uint_no64bmul(uint32_t *output, const uint32_t *input, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        unsigned char *in = (unsigned char *)&input[i];
        unsigned char *out = (unsigned char *)&output[i];

        out[0] = reverse_uchar_no64bmul(in[3]);
        out[1] = reverse_uchar_no64bmul(in[2]);
        out[2] = reverse_uchar_no64bmul(in[1]);
        out[3] = reverse_uchar_no64bmul(in[0]);
    }
}
void reverse_uint_log(uint32_t *output, const uint32_t *input, uint32_t size){
    for (uint32_t i= 0; i < size; i++) {
        uint32_t v = input[i];

        v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);
        v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);
        v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);
        v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);
        v = ( v >> 16             ) | ( v               << 16);
        output[i] = v;
    }
}

#define AVX256_UINT_WIDTH 8

void reverse_uint_log_avx256(uint32_t *output, const uint32_t *input, uint32_t size) {
    uint32_t i = 0;

    __m256i constant_5555 = _mm256_set1_epi32(0x55555555);
    __m256i constant_3333 = _mm256_set1_epi32(0x33333333);
    __m256i constant_0f0f = _mm256_set1_epi32(0x0f0f0f0f);
    __m256i constant_00ff = _mm256_set1_epi32(0x00ff00ff);

    for (; i < size; i+=AVX256_UINT_WIDTH) {
        __m256i in = _mm256_loadu_si256((__m256i *)&input[i]);
        __m256i temp, temp1, temp2;

        temp1 = _mm256_and_si256(_mm256_srli_epi32(in, 1), constant_5555);
        temp2 = _mm256_slli_epi32(_mm256_and_si256(in, constant_5555), 1);
        temp = _mm256_or_si256(temp1, temp2);

        temp1 = _mm256_and_si256(_mm256_srli_epi32(temp, 2), constant_3333);
        temp2 = _mm256_slli_epi32(_mm256_and_si256(temp, constant_3333), 2);
        temp = _mm256_or_si256(temp1, temp2);
        
        temp1 = _mm256_and_si256(_mm256_srli_epi32(temp, 4), constant_0f0f);
        temp2 = _mm256_slli_epi32(_mm256_and_si256(temp, constant_0f0f), 4);
        temp = _mm256_or_si256(temp1, temp2);

        temp1 = _mm256_and_si256(_mm256_srli_epi32(temp, 8), constant_00ff);
        temp2 = _mm256_slli_epi32(_mm256_and_si256(temp, constant_00ff), 8);
        temp = _mm256_or_si256(temp1, temp2);

        temp = _mm256_or_si256(_mm256_srli_epi32(temp, 16), _mm256_slli_epi32(temp, 16));

        _mm256_storeu_si256((__m256i *)&output[i], temp);
    }

    uint32_t remaining_size = size-size/AVX256_UINT_WIDTH*AVX256_UINT_WIDTH;
    reverse_uint_log(&output[i], &input[i], remaining_size);
}
