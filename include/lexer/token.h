#ifndef RISC_V_CROSS_COMPILER_TOKEN_H
#define RISC_V_CROSS_COMPILER_TOKEN_H
#include "stdbool.h"


enum Type_Of_Token {
    IDENTIFIER = 0,
    BINARY_INT_LITERAL = 1,
    OCTAL_INT_LITERAL = 2,
    HEX_INT_LITERAL = 3,
    DECIMAL_INT_LITERAL = 4,
    DOUBLE_LITERAL = 5,
    STRING_LITERAL = 6,
    CHAR_LITERAL = 7,
    CHAR = 8,
    INT = 9,
    FLOAT = 10,
    IF = 11,
    ELSE = 12,
    WHILE = 13,
    DO = 14,
    FOR = 15,
    SWITCH = 16,
    CASE = 17,
    CONTINUE = 18,
    BREAK = 19,
    DEFAULT = 20,
    RETURN = 21,
    CONST = 22,
    VOID = 23,
    ARITHMETIC_OPERATOR = 24,
    LOGIC_OPERATOR = 25,
    COMPARISON_OPERATOR = 26,
    BITWISE_OPERATOR = 27,
    ASSIGN_OPERATOR = 28,
    ARITHMETIC_ASSIGN_OPERATOR = 29,
    BITWISE_ASSIGN_OPERATOR = 30,
    LEFT_ROUND_BRACKET = 31,
    RIGHT_ROUND_BRACKET = 32,
    LEFT_CURLY_BRACKET = 33,
    RIGHT_CURLY_BRACKET = 34,
    LEFT_SQUARE_BRACKET = 35,
    RIGHT_SQUARE_BRACKET = 36,
    SEMICOLON = 37,
    COMMA = 38,
    INCORRECT = 39,
    END = 40,
    MULTIPLICATION_SIGN = 41,
};


struct Token_Attributes {
    char* text;
    unsigned long long text_length;
    unsigned long long line;
    unsigned long long column;
};


struct Token {
    enum Type_Of_Token type;
    struct Token_Attributes* attributes;
};


struct Token* init_token(enum Type_Of_Token type, char* text, unsigned long long text_length,
        unsigned long long line, unsigned long long column);


#endif //RISC_V_CROSS_COMPILER_TOKEN_H

