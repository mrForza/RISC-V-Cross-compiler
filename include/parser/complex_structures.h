#include "../lexer/token.h"

enum Type_Of_Grammars {
    SINGLE_DECLARATION = 0,
    COMPLEX_DECLARATION = 1,
    SINGLE_DEFINITION = 2,
    COMPLEX_DEFINITION = 3,
    ARITHMETIC_EXPRESSION = 4,
    LOGIC_EXPRESSION = 5,
    RELATIONAL_EXPRESSION = 6,
    IF_ELSE_STATEMENT = 7,
    DO_WHILE_STATEMENT = 8,
    WHILE_STATEMENT = 9,
    FOR_STATEMENT = 10,
    FUNCTION_DECLARATION = 11
};


struct Grammar {
    void* data;
    enum Type_Of_Grammars type;
};


struct Arithmetic_Expression {
    void* left_operand;
    char* operator; // ARITHMETIC_OPERATOR OR BITWISE_OPERATOR
    void* right_operand;
    bool is_left_expr;
    bool is_right_expr;
};


struct Logic_Expression {
    void* left_operand;
    char* operator; // LOGIC_OPERATOR
    void* right_operand;
    bool is_left_expr;
    bool is_right_expr;
};


struct Relational_Expression {
    void *left_operand;
    char* operator; // RELATIONAL_OPERATOR
    void *right_operand;
    bool is_left_expr;
    bool is_right_expr;
};


struct Single_Declaration {
    char* type;
    char* var_name;
};


struct Complex_Declaration {
    char* type;
    char** var_names;
};


struct Single_Definition {
    char* type;
    char* var_name;
    void* expression;
    bool is_expression;
};


struct Complex_Definition {
    char* type;
    char** var_names;
    void** expressions;
};


struct If_Else {
    void** conditions; // n
    void** bodies; // n + 1
    bool is_else_exists;
};


struct While {

};


struct Do_While {

};


struct For {

};


int find_close_curly_bracket(void** tokens, int start_position, int size);


int find_nearest_semicolon(void** tokens, int start_position, int size);


int find_close_bracket(void** tokens, int start_position, int size);


bool is_arithmetic_expression(void** tokens, const int* start_position, int size);


bool is_single_declaration_expression(void** tokens, const int* start_position, int size);


bool is_complex_declaration_expression(void** tokens, const int* start_position, int size);


bool is_single_definition_expression(void** tokens, const int* start_position, int size);


bool is_complex_definition_expression(void** tokens, const int* start_position, int size);


bool is_logical_expression(void** tokens, const int* start_position, int size);


bool is_relational_expression(void** tokens, const int* start_position, int size);


bool is_bitwise_expression(void** tokens, const int* start_position, int size);


bool is_if_else_statement(void** tokens, const int* start_position, int size);


bool is_while_statement(void** tokens, const int* start_position, int size);


bool is_do_while_statement(void** tokens, const int* start_position, int size);


bool is_for_statement(void** tokens, const int* start_position, int size);


bool is_prefix_expression(void** tokens, const int* start_position, int size);


bool is_postfix_expression(void** tokens, const int* start_position, int size);