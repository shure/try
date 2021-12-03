
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include <algorithm>

void radix_sort(uint32_t* array, size_t n)
{
    uint32_t* second = new uint32_t[n];
    
    uint32_t* current = array;
    uint32_t* next = second;

    for (int bit = 0; bit < 32; bit++) {

        uint32_t bit_mask = 1U << bit;

        uint32_t* outp = next;

        /* Copy elements having zero 'bit' value. */
        for (size_t i = 0; i < n; i++) {
            if ((current[i] & bit_mask) == 0) {
                *outp++ = current[i];
            }
        }

        /* Copy elements having 'one' bit value. */
        for (size_t i = 0; i < n; i++) {
            if ((current[i] & bit_mask) != 0) {
                *outp++ = current[i];
            }
        }

        std::swap(current, next);
    }

    delete second;
}

void radix_qsort(uint32_t* array, size_t n, uint32_t bit_mask)
{
    if (n <= 1 || !bit_mask) {
        return;
    }

    size_t i = 0, tail = n;
    while (i < tail) {
        if (array[i] & bit_mask) {
            tail--;            
            std::swap(array[i], array[tail]);
        } else {
            i++;
        }
    }

    radix_qsort(array, tail, bit_mask >> 1);
    radix_qsort(array + tail, n - tail, bit_mask >> 1);
}

void test()
{
    size_t N = 10000000;
    uint32_t* array = new uint32_t[N];

    for (size_t i = 0; i < N; i++)
        array[i] = random();
    
    // radix_sort(array, N);
    radix_qsort(array, N, 1U << 31);

    // std::sort(array, array + N);

    for (size_t i = 1; i < N; i++)
        assert(array[i-1] <= array[i]);

    delete array;
}

int main()
{
    for (int i = 0; i < 10; i++) {
        test();
    }
}
