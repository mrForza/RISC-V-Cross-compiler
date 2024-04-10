#include "vector.h"
#include "stdlib.h"


struct Vector* init_vector(int capacity, enum Type_Of_Items type) {
    struct Vector* vector = (struct Vector*)malloc(1 * sizeof(struct Vector));
    vector->size = 0;
    vector->capacity = capacity;
    vector->type = type;

    if (vector->type == INT) {
        vector->data = (void*)(int**)malloc(vector->capacity * sizeof(int*));
    } else {
        vector->data = (void*)(struct Token**)malloc(vector->capacity * sizeof(struct Token*));
    }

    for (int i = 0; i < vector->capacity; ++i) {
        vector->data[i] = NULL;
    }

    return vector;
}


void push_back_token(struct Vector* vector, struct Token* token) {
    if (vector->size >= vector->capacity) {
        void** new_data = realloc(vector->data, 2 * vector->capacity);
        vector->data = new_data;
        vector->capacity *= 2;
    }
    vector->data[vector->size++] = token;
}


void push_back_integer(struct Vector* vector, size_t number) {
    if (vector->size >= vector->capacity) {
        void** new_data = realloc(vector->data, 2 * vector->capacity);
        vector->data = new_data;
        vector->capacity *= 2;
    }
    size_t* ref_to_number = (size_t*)malloc(sizeof(size_t));
    *ref_to_number = number;
    vector->data[vector->size++] = ref_to_number;
}


void destroy_vector(struct Vector* vector) {
    for (int i = 0; i < vector->size; ++i) {
        free(vector->data[i]);
    }
    free(vector);
}