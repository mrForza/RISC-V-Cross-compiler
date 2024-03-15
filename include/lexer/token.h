#ifndef RISC_V_CROSS_COMPILER_TOKEN_H
#define RISC_V_CROSS_COMPILER_TOKEN_H
#include "stdbool.h"


enum Type_Of_Token {
    IDENTIFIER = 0, // A non-keyword word which matches with this regex: [_a-zA-Z][_a-zA-Z0-9]{0,30}
    STRING_LITERAL = 1, // "Lorem ipsum"
    SYMBOL_LITERAL = 2, // 'a'
    BINARY_INTEGER_LITERAL = 99,
    OCTAL_INTEGER_LITERAL = 4, // 0o76543210
    DECIMAL_INTEGER_LITERAL = 3, // 123, 123l, 123u, 1233L, 123U, 123lu, 123LU, 123lU, 123Ul
    HEXADECIMAL_INTEGER_LITERAL = 5, // 0xfedcba987654321
    DECIMAL_REAL_LITERAL = 6, // 123.123, 123.123f, 123.123F, 123.123l, 123.123L
    EXPONENTIAL_REAL_LITERAL = 7, // 12.2e+1, 12.2E+1, 12.2e-1, 12.2E-1, 123e+1, 123e-1, 123E+1, 123E-1
    HEXADECIMAL_REAL_LITERAL = 8, // 0x123a.c0fp+1, 0x123a.c0fP+1, 0x123a.c0fp-1, 0x123a.c0fP-1
    INTEGER_DATA_TYPE = 9, // char, short, int, long
    SIGN_MODIFIER = 10, // signed, unsigned
    REAL_DATA_TYPE = 11, // float, double
    LOOP = 12, // do, while, for
    CONDITIONAL_OPERATOR = 13, // if, else
    TRANSITION_INTERRUPT_OPERATOR = 14, // continue, break, return, goto
    COMPLEX_DATA_TYPE = 15, // struct, union, enum
    ARITHMETIC_OPERATOR = 16, // +, -, *, /, %, ++, --
    LOGIC_OPERATOR = 17, // ||, &&, !
    COMPARISON_OPERATOR = 18, // <, <=, >, >=, ==, !=
    BITWISE_OPERATOR = 19, // |, &, ~, <<, >>
    ASSIGN_OPERATOR = 20, // =
    COMPLEX_ASSIGN_OPERATOR = 21, // +=, -=, *=, /=, %=, |=, &=, ~=, <<=, >>=
    ROUND_BRACKET = 22, // (, )
    SQUARE_BRACKET = 23, // [, ]
    CURLY_BRACKET = 24, // {, }
    TRIANGLE_BRACKET = 25, // <, >
    HASH = 26, // #
    CONST = 27,
    STATIC = 28,
    VOID = 29,
    TYPEDEF = 30,
    SIZEOF = 31,
    DEFINE = 32,
    INCLUDE = 33,
    INCORRECT = 34,
    END = 35,
    CHAR = 36,
    CASE = 37,
    ENUM = 38,
    ELSE = 39,
    FOR = 40,
    FLOAT = 41,
    IF = 42,
    INTEGER = 43,
    UNION = 44,
    UNSIGNED = 45,
    SIGNED = 46,
    STRUCT = 47,
    SHORT = 48,
    SWITCH = 49,
    LEFT_ROUND_BRACKET = 50,
    RIGHT_ROUND_BRACKET = 51,
    LEFT_SQUARE_BRACKET = 52,
    RIGHT_SQUARE_BRACKET = 53,
    LEFT_CURLY_BRACKET = 54,
    RIGHT_CURLY_BRACKET = 55,
    LEFT_TRIANGLE_BRACKET = 56,
    RIGHT_TRIANGLE_BRACKET = 57,
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

