#include "stdlib.h"
#include "string.h"


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


char* concatenate(const char* first_string, const char* second_string) {
    strlen(first_string);
    strlen(second_string);
    char* result_string = (char*)malloc(strlen(first_string) + strlen(second_string) + 1);
    strcpy(result_string, first_string);
    strcat(result_string, second_string);
    return result_string;
}


char* concatenate_string_int(const char* string, unsigned long long number) {
    unsigned long long temp_number = number;
    unsigned long long count_of_bits = 0;
    while (number > 0) {
        number /= 10;
        ++count_of_bits;
    }

    char* number_representation = (char*)malloc(temp_number * sizeof(char));
    sprintf(number_representation, "%llu", temp_number);

    char* result_string = (char*)malloc(strlen(string) + strlen(number_representation) + 1);
    strcpy(result_string, string);
    strcat(result_string, number_representation);
    return result_string;
}