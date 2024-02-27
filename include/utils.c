#include "stdlib.h"


char* get_substring(const char* source, unsigned long long left_idx, unsigned long long right_idx) {
    char* destination = (char*)malloc(right_idx - left_idx + 2);
    unsigned long long j = 0;
    for (unsigned long long i = left_idx; i <= right_idx; ++i) {
        destination[j] = source[i];
        ++j;
    }
    destination[j] = '\0';
    return destination;
}