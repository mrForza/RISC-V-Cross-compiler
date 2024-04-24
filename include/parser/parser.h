#include "../lexer/lexer.c"


struct Parser {
    struct Vector* tokens;
};


struct Parser init_parser(struct Lexer* lexer);


struct Grammar* start_parsing(struct Parser* parser, int* quantity_of_grammars, int* is_error);


struct Arithmetic_Expression get_arithmetic_expression(void** tokens, int start, int end, int* is_error);


struct Logic_Expression get_logic_expression(void** tokens, int start, int end, int* is_error);


struct Relational_Expression get_relational_expression(void** tokens, int start, int end, int* is_error);


struct Single_Declaration get_single_declaration(void** tokens, int start, int end, int* is_error);


struct Complex_Declaration get_complex_declaration(void** tokens, int start, int end, int* is_error);


struct Single_Definition get_single_definition(void** tokens, int start, int end, int* is_error);


struct Complex_Definition get_complex_definition(void** tokens, int start, int end, int* is_error);


struct If_Else get_if_else_statement(void** tokens, int start, int end, int* is_error);


struct While get_while_statement(void** tokens, int start, int end, int* is_error);


struct Do_While get_do_while_statement(void** tokens, int start, int end, int* is_error);


struct For get_for_statement(void** tokens, int start, int end, int* is_error);


struct Assignment_Expression get_assignment_expression(void** tokens, int start, int end, int* is_error);


struct Read_Int_Function get_read_int(void** tokens, int start, int end, int* is_error);


struct Write_Int_Function get_write_int(void** tokens, int start, int end, int* is_error);


struct General_Assignment_Expression get_general_assignment_expression(
        void** tokens, int start, int end, int* is_error);


struct Read_Float_Function get_read_float(void** tokens, int start, int end, int* is_error);


struct Write_Float_Function get_write_float(void** tokens, int start, int end, int* is_error);