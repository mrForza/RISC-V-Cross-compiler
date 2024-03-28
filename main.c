#include "stdio.h"
#include "include/utils/vector.c"
#include "include/parser/complex_structures.c"

int main() {
    struct Vector* vector = init_vector(1024, TOKEN);
    push_back_token(vector, init_token(INT, "int", 0, 0, 0));
    push_back_token(vector, init_token(MULTIPLICATION_SIGN, "*", 0, 0, 0));
    push_back_token(vector, init_token(IDENTIFIER, "a", 0, 0, 0));
    push_back_token(vector, init_token(COMMA, ",", 0, 0, 0));
    push_back_token(vector, init_token(IDENTIFIER, "b", 0, 0, 0));
    push_back_token(vector, init_token(COMMA, ",", 0, 0, 0));
    push_back_token(vector, init_token(IDENTIFIER, "c", 0, 0, 0));
    push_back_token(vector, init_token(COMMA, ",", 0, 0, 0));
    push_back_token(vector, init_token(IDENTIFIER, "d", 0, 0, 0));

    size_t start_position = 0;
    size_t* pointer = &start_position;
    printf("%d", is_complex_declaration_expression(vector->data, pointer, vector->size));
    return 0;
}
