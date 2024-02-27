#include "stdio.h"
#include "include/lexer/lexer.c"

int main() {
    FILE* file = fopen("../tests/test_2.txt", "r");
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        unsigned long long buffer_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        char* buffer = malloc(buffer_size + 1);
        fread(buffer, buffer_size, 1, file);
        fclose(file);
        struct Lexer lexer = init_lexer(buffer, buffer_size);
        struct Vector* vector = get_all_tokens(&lexer);

        unsigned long long index = 0;
        struct Token* curr_token = ((struct Token*)(vector->data[index]));
        while (curr_token != NULL && curr_token->type != END) {
            switch (curr_token->type) {
                case BINARY_INTEGER_LITERAL:
                    printf("BINARY_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case OCTAL_INTEGER_LITERAL:
                    printf("OCTAL_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case DECIMAL_INTEGER_LITERAL:
                    printf("DECIMAL_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case HEXADECIMAL_INTEGER_LITERAL:
                    printf("HEXADECIMAL_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case DECIMAL_REAL_LITERAL:
                    printf("DECIMAL_REAL_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case EXPONENTIAL_REAL_LITERAL:
                    printf("EXPONENTIAL_REAL_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case SYMBOL_LITERAL:
                    printf("SYMBOL_LITERAL: %s\n", curr_token->attributes->text);
                    break;
                case STRING_LITERAL:
                    printf("STRING_LITERAL: %s\n", curr_token->attributes->text);
                    break;
                case INCORRECT:
                    printf("INCORRECT_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                default:
                    break;
            }
            curr_token = ((struct Token *) (vector->data[++index]));
        }

        destroy_vector(vector);
    }
    return 0;
}
