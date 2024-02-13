#include "lexer.h"


struct Lexer init_lexer(const char* content, unsigned long long content_length) {
    struct Lexer lexer = {};
    lexer.content = content;
    lexer.content_length = content_length;
    lexer.main_cursor = 0;
    lexer.forward_cursor = 0;
    return lexer;
}


struct Token get_next_token(struct Lexer* lexer) {
    struct Token token = {};

    while (lexer->content[lexer->main_cursor] != '\0') { // Similar to EOF

    }

    return token;
}