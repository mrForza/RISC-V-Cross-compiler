#include "stdio.h"
#include "include/parser/complex_structures.c"
#include "include/parser/parser.c"
#include "include/codegen/code_generator.c"

int main() {
    struct Vector* vector = init_vector(1024, TOKEN);
    push_back_token(vector, init_token(LEFT_ROUND_BRACKET, "(", 0, 0, 0));
    push_back_token(vector, init_token(LEFT_ROUND_BRACKET, "(", 0, 0, 0));
    push_back_token(vector, init_token(LEFT_ROUND_BRACKET, "(", 0, 0, 0));
    push_back_token(vector, init_token(DECIMAL_INTEGER_LITERAL, "3", 0, 0, 0));
    push_back_token(vector, init_token(BITWISE_OPERATOR, "&", 0, 0, 0));
    push_back_token(vector, init_token(DECIMAL_INTEGER_LITERAL, "12", 0, 0, 0));
    push_back_token(vector, init_token(RIGHT_ROUND_BRACKET, ")", 0, 0, 0));
    push_back_token(vector, init_token(BITWISE_OPERATOR, "+", 0, 0, 0));
    push_back_token(vector, init_token(DECIMAL_INTEGER_LITERAL, "1", 0, 0, 0));
    push_back_token(vector, init_token(RIGHT_ROUND_BRACKET, ")", 0, 0, 0));
    push_back_token(vector, init_token(BITWISE_OPERATOR, "*", 0, 0, 0));
    push_back_token(vector, init_token(DECIMAL_INTEGER_LITERAL, "8", 0, 0, 0));
    push_back_token(vector, init_token(RIGHT_ROUND_BRACKET, ")", 0, 0, 0));
    push_back_token(vector, init_token(BITWISE_OPERATOR, "|", 0, 0, 0));
    push_back_token(vector, init_token(LEFT_ROUND_BRACKET, "(", 0, 0, 0));
    push_back_token(vector, init_token(DECIMAL_INTEGER_LITERAL, "7", 0, 0, 0));
    push_back_token(vector, init_token(ARITHMETIC_OPERATOR, "+", 0, 0, 0));
    push_back_token(vector, init_token(DECIMAL_INTEGER_LITERAL, "6", 0, 0, 0));
    push_back_token(vector, init_token(RIGHT_ROUND_BRACKET, ")", 0, 0, 0));

    size_t start_position = 0;
    size_t* pointer = &start_position;

    if (is_bitwise_expression(vector->data, pointer, vector->size - 1)) {
        struct Bitwise_Expression expression = get_bitwise_expression(vector->data, 0, vector->size - 1);
        printf("%s", generate_assembly_for_bitwise_expression(expression));
    }

    return 0;
}
