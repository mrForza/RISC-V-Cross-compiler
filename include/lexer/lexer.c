#include "lexer.h"


struct Lexer init_lexer(const char* content, unsigned long long content_length) {
    struct Lexer lexer = {};
    lexer.content = content;
    lexer.content_length = content_length;
    return lexer;
}


struct Token get_next_token(struct Lexer* lexer) {
    struct Token token = {};

    return token;
}