#ifndef RISC_V_CROSS_COMPILER_LEXER_H
#define RISC_V_CROSS_COMPILER_LEXER_H
#include "token.h"

struct Lexer {
    char* content;
    unsigned long long content_length;
};


struct Lexer init_lexer(char* content, unsigned long long content_length);


struct Vector* get_all_tokens(struct Lexer* lexer);


struct Token* get_token(char* content, unsigned long long position);


struct Token* get_number_token(char* content, unsigned long long position);


struct Token* determine_hexadecimal_token(char* content, unsigned long long position);


struct Token* determine_octal_token(char* content, unsigned long long position);


struct Token* determine_binary_token(char* content, unsigned long long position);


struct Token* determine_decimal_token(char* content, unsigned long long position);


struct Token* determine_real_token(char* content, unsigned long long start_position, unsigned long long position);


struct Token* determine_exponential_real_token(char* content, unsigned long long position);


struct Token* get_keyword_token(char* content, unsigned long long position);


struct Token* get_other_token(char* content, unsigned long long position);


struct Token* get_identifier_token(char* content, unsigned long long position);


struct Token* get_string_literal_token(char* content, unsigned long long position);


struct Token* get_char_literal_token(char* content, unsigned long long position);


#endif //RISC_V_CROSS_COMPILER_LEXER_H
