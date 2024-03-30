#include "../lexer/token.h"
#include "../utils/vector.h"


struct Arithmetic_Expression {
    void* left_operand;
    char* operator; // ARITHMETIC_OPERATOR
    void* right_operand;
    bool is_left_expr;
    bool is_right_expr;
};


struct Logic_Expression {
    void* left_operand;
    struct Token token; // LOGIC_OPERATOR
    void* right_operand;
};


struct Bitwise_Expression {
    void *left_operand;
    char* operator; // BITWISE_OPERATOR
    void *right_operand;
    int left_expr;
    int right_expr; // 0 - Number, 1 - Bitwise Expr, 2 - Arithmetic Expr
};


struct Relational_Expression {
    void *left_operand;
    struct Token token; // RELATIONAL_OPERATOR
    void *right_operand;
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
    char* value;
};


struct Complex_Definition {
    char* type;
    char** var_names;
    char** values;
};


size_t find_close_bracket(void** tokens, size_t start_position, size_t size);


bool is_arithmetic_expression(void** tokens, const size_t* start_position, size_t size);


bool is_single_declaration_expression(void** tokens, const size_t* start_position, size_t size);


bool is_complex_declaration_expression(void** tokens, const size_t* start_position, size_t size);


bool is_single_definition_expression(void** tokens, const size_t* start_position, size_t size);


bool is_complex_definition_expression(void** tokens, const size_t* start_position, size_t size);


bool is_logical_expression(void** tokens, const size_t* start_position, size_t size);


bool is_relational_expression(void** tokens, const size_t* start_position, size_t size);


bool is_bitwise_expression(void** tokens, const size_t* start_position, size_t size);

