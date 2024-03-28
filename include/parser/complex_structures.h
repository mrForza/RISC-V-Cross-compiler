#include "../lexer/token.h"
#include "../utils/vector.h"


struct Arithmetic_Operand {
    void* data; // Number or Arithmetic_Operand
};


struct Arithmetic_Expression {
    struct Arithmetic_Operand left_operand;
    struct Token token;
    struct Arithmetic_Operand right_operand;
};


bool is_expression(struct Vector* tokens);


bool is_arithmetic_expression(struct Vector* tokens);


bool is_assignment_expression(struct Vector* tokens);


bool is_conditional_expression(struct Vector* tokens);


bool is_logical_expression(void** tokens, size_t* start_position, size_t size);


bool is_relational_expression(struct Vector* tokens);


bool is_bitwise_expression(struct Vector* tokens);

