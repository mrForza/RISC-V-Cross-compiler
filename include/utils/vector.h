#ifndef RISC_V_CROSS_COMPILER_VECTOR_H
#define RISC_V_CROSS_COMPILER_VECTOR_H
#include "../lexer/token.c"


enum Type_Of_Items {
    NUMBER,
    TOKEN,
    GRAMMAR
};


struct Vector {
    void** data;
    int size;
    int capacity;
    enum Type_Of_Items type;
};


struct Vector* init_vector(int capacity, enum Type_Of_Items);


void push_back_token(struct Vector* vector, struct Token* token);


void push_back_integer(struct Vector* vector, size_t number);


void destroy_vector(struct Vector* vector);


#endif //RISC_V_CROSS_COMPILER_VECTOR_H
