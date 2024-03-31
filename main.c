#include "stdio.h"
#include "include/parser/complex_structures.c"
#include "include/parser/parser.c"
#include "include/codegen/code_generator.c"

int main() {
    struct Vector* vector = init_vector(1024, TOKEN);
    push_back_token(vector, init_token(FOR, "for", 0, 0, 0));
    push_back_token(vector, init_token(LEFT_ROUND_BRACKET, "(", 0, 0, 0));
    push_back_token(vector, init_token(INT, "int", 0, 0, 0));
    push_back_token(vector, init_token(IDENTIFIER, "i", 0, 0, 0));
    push_back_token(vector, init_token(ASSIGN_OPERATOR, "=", 0, 0, 0));
    push_back_token(vector, init_token(DECIMAL_INTEGER_LITERAL, "5", 0, 0, 0));
    push_back_token(vector, init_token(SEMICOLON, ";", 0, 0, 0));
    push_back_token(vector, init_token(IDENTIFIER, "i", 0, 0, 0));
    push_back_token(vector, init_token(CONDITIONAL_OPERATOR, "<", 0, 0, 0));
    push_back_token(vector, init_token(DECIMAL_INTEGER_LITERAL, "10", 0, 0, 0));
    push_back_token(vector, init_token(SEMICOLON, ";", 0, 0, 0));
    push_back_token(vector, init_token(IDENTIFIER, "i", 0, 0, 0));
    push_back_token(vector, init_token(IDENTIFIER, "++", 0, 0, 0));
    push_back_token(vector, init_token(RIGHT_ROUND_BRACKET, ")", 0, 0, 0));
    push_back_token(vector, init_token(LEFT_CURLY_BRACKET, "{", 0, 0, 0));
    push_back_token(vector, init_token(DECIMAL_INTEGER_LITERAL, "a", 0, 0, 0));
    push_back_token(vector, init_token(ASSIGN_OPERATOR, "=", 0, 0, 0));
    push_back_token(vector, init_token(DECIMAL_INTEGER_LITERAL, "3", 0, 0, 0));
    push_back_token(vector, init_token(RIGHT_CURLY_BRACKET, "}", 0, 0, 0));


    size_t start_position = 0;
    size_t* pointer = &start_position;

    if (is_for_statement(vector->data, pointer, vector->size - 1)) {
       printf("YES");
    }

    return 0;
}
