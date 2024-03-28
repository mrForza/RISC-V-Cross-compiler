#include "stdio.h"
#include "../include/codegen/code_generator.c"


int main() {
    printf("%s\n", generate_assembly_for_var_declaration("int", "var"));
    printf("%s\n", generate_assembly_for_var_definition("int", "a", "123"));
    printf("%s\n", generate_assembly_for_var_definition("int", "pp", "-123241"));
    char* var_names[3] = {"first", "second", "third"};
    char* var_values[3] = {"1", "2", "3"};
    printf("%s\n", generate_assembly_for_variables_definitions("char", var_names, var_values, 3));
    printf("\n");
    /*
    FILE* file = fopen("../tests/test_3.txt", "r");
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
                case LOOP:
                    printf("LOOP_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case TRANSITION_INTERRUPT_OPERATOR:
                    printf("INTERRUPT_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case CONST:
                    printf("CONST_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case CONDITIONAL_OPERATOR:
                    printf("CONDITIONAL_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case REAL_DATA_TYPE:
                    printf("REAL_DATA_TYPE_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case COMPLEX_DATA_TYPE:
                    printf("COMPLEX_DATA_TYPE_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case INTEGER_DATA_TYPE:
                    printf("INTEGER_DATA_TYPE_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case TYPEDEF:
                    printf("TYPEDEF_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case VOID:
                    printf("VOID_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case CHAR:
                    printf("CHAR_DATA_TYPE_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case CASE:
                    printf("CASE_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case ENUM:
                    printf("ENUM_DATA_TYPE_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case ELSE:
                    printf("ELSE_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case FOR:
                    printf("FOR_TYPE_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case FLOAT:
                    printf("FLOAT_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case IF:
                    printf("IF_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case INTEGER:
                    printf("INT_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case UNION:
                    printf("UNION_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case UNSIGNED:
                    printf("UNSIGNED_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case STRUCT:
                    printf("STRUCT_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case SIZEOF:
                    printf("SIZEOF_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case SHORT:
                    printf("SHORT_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case SIGNED:
                    printf("SIGNED_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case SWITCH:
                    printf("SWITCH_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case STATIC:
                    printf("STATIC_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case ARITHMETIC_OPERATOR:
                    printf("ARITHMETIC_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case COMPARISON_OPERATOR:
                    printf("COMPARISON_TOKEN: %s\n", curr_token->attributes->text);
                    break;
                case LEFT_ROUND_BRACKET:
                    printf("LEFT_ROUND_BRACKET: %s\n", curr_token->attributes->text);
                    break;
                case RIGHT_ROUND_BRACKET:
                    printf("RIGHT_ROUND_BRACKET: %s\n", curr_token->attributes->text);
                    break;
                case LEFT_SQUARE_BRACKET:
                    printf("LEFT_SQUARE_BRACKET: %s\n", curr_token->attributes->text);
                    break;
                case RIGHT_SQUARE_BRACKET:
                    printf("RIGHT_SQUARE_BRACKET: %s\n", curr_token->attributes->text);
                    break;
                case IDENTIFIER:
                    printf("IDENTIFIER: %s\n", curr_token->attributes->text);
                    break;
                default:
                    break;
            }
            curr_token = ((struct Token *) (vector->data[++index]));
        }

        destroy_vector(vector);
    }
     */
    return 0;
}