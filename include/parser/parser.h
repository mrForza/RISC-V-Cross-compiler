#include "complex_structures.c"
#include "../lexer/lexer.c"


struct Parser {
    struct Vector* tokens;
};


struct Parser init_parser(struct Lexer* lexer);


struct Grammar* start_parsing(struct Parser* parser, int* quantity_of_grammars);


struct Arithmetic_Expression get_arithmetic_expression(void** tokens, int start, int end);


struct Logic_Expression get_logic_expression(void** tokens, int start, int end);


struct Relational_Expression get_relational_expression(void** tokens, int start, int end);


struct Single_Declaration get_single_declaration(void** tokens, int start, int end);


struct Complex_Declaration get_complex_declaration(void** tokens, int start, int end);


struct Single_Definition get_single_definition(void** tokens, int start, int end);


struct Complex_Definition get_complex_definition(void** tokens, int start, int end);


struct If_Else get_if_else_statement(void** tokens, int start, int end);


struct While get_while_statement(void** tokens, int start, int end);


struct Do_While get_do_while_statement(void** tokens, int start, int end);


struct For get_for_statement(void** tokens, int start, int end);