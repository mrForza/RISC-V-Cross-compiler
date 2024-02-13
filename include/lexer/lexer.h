#ifndef RISC_V_CROSS_COMPILER_LEXER_H
#define RISC_V_CROSS_COMPILER_LEXER_H
#include "token.h"

struct Lexer {
    const char* content;
    unsigned long long content_length;
    unsigned long long main_cursor;
    unsigned long long forward_cursor;
};


struct Lexer init_lexer(const char* content, unsigned long long content_length);


struct Token get_next_token(struct Lexer* lexer);


void skip_white_spaces(struct Lexer* lexer);


void move_cursor(struct Lexer* lexer);


#endif //RISC_V_CROSS_COMPILER_LEXER_H
