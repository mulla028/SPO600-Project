#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__attribute__((target_clones("default", "simd")))
int simple_calculation(int a, int b, int c) {
    return (a + b) * c / (a - b + 1);
}

__attribute__((target_clones("default", "sve")))
void vector_multiply(float *result, const float *a, const float *b, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = a[i] * b[i];
    }
}

__attribute__((target_clones("default", "sve")))
void matrix_transpose(float *dst, const float *src, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[j * rows + i] = src[i * cols + j];
        }
    }
}

__attribute__((target_clones("default", "crc")))
int count_bits(uint64_t value) {
    int count = 0;
    while (value) {
        count += (value & 1);
        value >>= 1;
    }
    return count;
}

__attribute__((target_clones("default", "simd")))
int count_char(const char *str, char c) {
    int count = 0;
    while (*str) {
        if (*str == c) count++;
        str++;
    }
    return count;
}

__attribute__((target_clones("default", "lse")))
int classify_number(int x) {
    if (x < 0) {
        if (x % 2 == 0) {
            return -2;
        } else {
            return -1;
        }
    } else if (x == 0) {
        return 0;
    } else {
        if (x % 2 == 0) {
            if (x % 4 == 0) {
                return 4;
            } else {
                return 2;
            }
        } else {
            if (x % 3 == 0) {
                return 3;
            } else {
                return 1;
            }
        }
    }
}

int main() {
    printf("=== Complex Test for Function Multi-Versioning Analysis (AArch64) ===\n\n");

    int result1 = simple_calculation(10, 5, 3);
    printf("Simple calculation result: %d\n", result1);

    float vec_a[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    float vec_b[8] = {0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0};
    float vec_result[8];
    vector_multiply(vec_result, vec_a, vec_b, 8);
    printf("Vector multiply first few results: %.1f %.1f %.1f ...\n",
           vec_result[0], vec_result[1], vec_result[2]);

    float matrix[9] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    float transposed[9];
    matrix_transpose(transposed, matrix, 3, 3);
    printf("Matrix transpose first row: %.1f %.1f %.1f\n",
           transposed[0], transposed[1], transposed[2]);

    uint64_t bits_value = 0x123456789ABCDEF0;
    int bits_count = count_bits(bits_value);
    printf("Bit count: %d\n", bits_count);

    const char *test_string = "This is a test string with some repeated characters";
    int char_count = count_char(test_string, 's');
    printf("Count of 's' in test string: %d\n", char_count);

    int classify_results[5];
    for (int i = -2; i <= 2; i++) {
        classify_results[i+2] = classify_number(i);
    }
    printf("Classification results from -2 to 2: %d %d %d %d %d\n",
           classify_results[0], classify_results[1], classify_results[2],
           classify_results[3], classify_results[4]);

    printf("\nAll tests completed!\n");
    return 0;
}
