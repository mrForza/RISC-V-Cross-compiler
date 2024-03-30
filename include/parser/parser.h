#include "../lexer/token.h"
#include "../lexer/lexer.h"


struct Parser {
    struct Vector* tokens;
};


struct Parser init_parser(struct Lexer* lexer);


struct Vector* start_parsing(struct Parser* parser);


struct Arithmetic_Expression get_arithmetic_expression(void** tokens, size_t start, size_t end);


struct Logic_Expression get_logic_expression(struct Vector vector);


struct Bitwise_Expression get_bitwise_expression(void** tokens, size_t start, size_t end);


struct Relational_Expression get_relational_expression(struct Vector vector);


struct Single_Declaration get_single_declaration(struct Vector vector);


struct Complex_Declaration get_complex_declaration(struct Vector vector);


struct Single_Definition get_single_definition(struct Vector vector);


struct Complex_Definition get_complex_definition(struct Vector vector);
