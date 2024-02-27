#ifndef RISC_V_CROSS_COMPILER_VECTOR_H
#define RISC_V_CROSS_COMPILER_VECTOR_H
#include "../lexer/token.c"


enum Type_Of_Items {
    INT,
    TOKEN
};


struct Vector {
    void** data;
    unsigned long long size;
    unsigned long long capacity;
    enum Type_Of_Items type;
};


struct Vector* init_vector(unsigned long long capacity, enum Type_Of_Items);


void push_back_token(struct Vector* vector, struct Token* token);


void destroy_vector(struct Vector* vector);


#endif //RISC_V_CROSS_COMPILER_VECTOR_H
