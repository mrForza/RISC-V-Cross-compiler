#include "parser.h"
#include "unistd.h"
#include "../utils/vector.h"


struct Parser init_parser(struct Lexer* lexer) {
    struct Parser parser = {};
    parser.tokens = get_all_tokens(lexer);
    return parser;
}


struct Vector* start_parsing(struct Parser* parser) {
    unsigned long long index = 0;
    void** tokens = parser->tokens->data;
    struct Token* curr_token = (struct Token*)(tokens[index]);
    struct Vector* wirths_grammars = init_vector(256, GRAMMAR);

    while (curr_token->type != END) {
        if (curr_token->type == SEMICOLON) {
            // Add grammar to vector
        } else {
            if (((struct Token*)(tokens[index - 1]))->type == SEMICOLON) {

            } else {

            }
        }

        curr_token = tokens[index++];
    }

    return wirths_grammars;
}