#include "../lexer/token.h"
#include "../lexer/lexer.h"


struct Parser {
    struct Vector* tokens;
};


struct Parser init_parser(struct Lexer* lexer);


struct Vector* start_parsing(struct Parser* parser);



