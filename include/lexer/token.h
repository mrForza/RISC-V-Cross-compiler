#ifndef RISC_V_CROSS_COMPILER_TOKEN_H
#define RISC_V_CROSS_COMPILER_TOKEN_H


enum Type_Of_Tokens {
    ID = 0
};

const char* a = "a";

struct Token_Attributes {
    const char* text;
    unsigned long long text_length;
    unsigned long long position;
};


struct Token {
    enum Type_Of_Tokens type;
    struct Token_Attributes attributes;
};


#endif //RISC_V_CROSS_COMPILER_TOKEN_H

