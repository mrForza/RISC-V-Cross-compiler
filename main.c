#include "stdio.h"
#include "include/file_handler/file_validator.c"
#include "include/file_handler/file_handler.c"
#include "include/preprocessor/preprocessor.c"
#include "include/parser/parser.c"
#include "include/codegen/code_generator.c"


int global_tab_counter = 0;

const char* GREETINGS_TEXT = "The simple Small-C preprocessor\n";

const char* LOGO = "\n"
                   "  ____  ___ ____   ____   __     __   ____                          ____                      _ _           \n"
                   " |  _ \\|_ _/ ___| / ___|  \\ \\   / /  / ___|_ __ ___  ___ ___       / ___|___  _ __ ___  _ __ (_) | ___ _ __ \n"
                   " | |_) || |\\___ \\| |   ____\\ \\ / /  | |   | '__/ _ \\/ __/ __|_____| |   / _ \\| '_ ` _ \\| '_ \\| | |/ _ \\ '__|\n"
                   " |  _ < | | ___) | |__|_____\\ V /   | |___| | | (_) \\__ \\__ \\_____| |__| (_) | | | | | | |_) | | |  __/ |   \n"
                   " |_| \\_\\___|____/ \\____|     \\_/     \\____|_|  \\___/|___/___/      \\____\\___/|_| |_| |_| .__/|_|_|\\___|_|   \n"
                   "                                                                                       |_|                  \n\n\n";

void print_all_tokens(struct Vector* tokens) {
    printf("\n");
    for (size_t i = 0; i < tokens->size; ++i) {
        switch (((struct Token*)(tokens->data[i]))->type) {
            case IDENTIFIER:
                printf("IDENTIFIER: ");
                break;
            case BINARY_INT_LITERAL:
                printf("BINARY_INT_LITERAL: ");
                break;
            case OCTAL_INT_LITERAL:
                printf("OCTAL_INT_LITERAL: ");
                break;
            case HEX_INT_LITERAL:
                printf("HEX_INT_LITERAL: ");
                break;
            case DOUBLE_LITERAL:
                printf("DOUBLE_LITERAL: ");
                break;
            case STRING_LITERAL:
                printf("STRING_LITERAL: ");
                break;
            case CHAR_LITERAL:
                printf("CHAR_LITERAL: ");
                break;
            case CHAR:
                printf("CHAR: ");
                break;
            case INT:
                printf("INT: ");
                break;
            case DOUBLE:
                printf("DOUBLE: ");
                break;
            case IF:
                printf("IF: ");
                break;
            case ELSE:
                printf("ELSE: ");
                break;
            case WHILE:
                printf("WHILE: ");
                break;
            case DO:
                printf("DO: ");
                break;
            case FOR:
                printf("FOR: ");
                break;
            case SWITCH:
                printf("SWITCH: ");
                break;
            case CASE:
                printf("CASE: ");
                break;
            case CONTINUE:
                printf("CONTINUE: ");
                break;
            case BREAK:
                printf("BREAK: ");
                break;
            case RETURN:
                printf("RETURN: ");
                break;
            case CONST:
                printf("CONST: ");
                break;
            case VOID:
                printf("VOID: ");
                break;
            case ARITHMETIC_OPERATOR:
                printf("ARITHMETIC_OPERATOR: ");
                break;
            case LOGIC_OPERATOR:
                printf("LOGIC_OPERATOR: ");
                break;
            case COMPARISON_OPERATOR:
                printf("COMPARISON_OPERATOR: ");
                break;
            case BITWISE_OPERATOR:
                printf("BITWISE_OPERATOR: ");
                break;
            case ASSIGN_OPERATOR:
                printf("ASSIGN_OPERATOR: ");
                break;
            case ARITHMETIC_ASSIGN_OPERATOR:
                printf("ARITHMETIC_ASSIGN_OPERATOR: ");
                break;
            case BITWISE_ASSIGN_OPERATOR:
                printf("BITWISE_ASSIGN_OPERATOR: ");
                break;
            case LEFT_ROUND_BRACKET:
                printf("LEFT_ROUND_BRACKET: ");
                break;
            case RIGHT_ROUND_BRACKET:
                printf("RIGHT_ROUND_BRACKET: ");
                break;
            case LEFT_CURLY_BRACKET:
                printf("LEFT_CURLY_BRACKET: ");
                break;
            case RIGHT_CURLY_BRACKET:
                printf("RIGHT_CURLY_BRACKET: ");
                break;
            case LEFT_SQUARE_BRACKET:
                printf("LEFT_SQUARE_BRACKET: ");
                break;
            case RIGHT_SQUARE_BRACKET:
                printf("RIGHT_SQUARE_BRACKET: ");
                break;
            case DECIMAL_INT_LITERAL:
                printf("DECIMAL_INT_LITERAL: ");
                break;
            case DEFAULT:
                printf("DEFAULT: ");
                break;
            case SEMICOLON:
                printf("SEMICOLON: ");
                break;
            case COMMA:
                printf("COMMA: ");
                break;
            case INCORRECT:
                printf("INCORRECT: ");
                break;
            case END:
                printf("THE END OF FILE");
                return;
            case MULTIPLICATION_SIGN:
                printf("MULTIPLICATION_SIGN: ");
                break;
        }
        printf("%s\n", ((struct Token*)(tokens->data[i]))->attributes->text);
    }
}


void print_syntax_constructions(struct Grammar* grammars, size_t size) {
    printf("\n");
    for (size_t i = 0; i < size; ++i) {
        struct If_Else if_else_statement;
        struct While while_statement;
        struct Do_While do_while_statement;
        struct Function_Declaration function_declaration;
        switch (grammars[i].type) {
            case SINGLE_DECLARATION:
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "SINGLE_DECLARATION\n\n"));
                break;
            case COMPLEX_DECLARATION:
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "COMPLEX_DECLARATION\n\n"));
                break;
            case SINGLE_DEFINITION:
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "SINGLE_DEFINITION\n\n"));
                break;
            case COMPLEX_DEFINITION:
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "COMPLEX_DEFINITION\n\n"));
                break;
            case ARITHMETIC_EXPRESSION:
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "ARITHMETIC_EXPRESSION\n\n"));
                break;
            case LOGIC_EXPRESSION:
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "LOGIC_EXPRESSION\n\n"));
                break;
            case RELATIONAL_EXPRESSION:
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "RELATIONAL_EXPRESSION\n\n"));
                break;
            case IF_ELSE_STATEMENT:
                if_else_statement = *((struct If_Else*)grammars[i].data);
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "IF_ELSE_STATEMENT ("));
                ++global_tab_counter;
                for (int j = 0; j < if_else_statement.quantity_of_conditions; ++j) {
                    if (j == 0) {
                        printf("LOGIC_EXPRESSION)\n");
                    } else {
                        printf("%s", concatenate(multiply_string("\t", global_tab_counter - 1), "ELSE_IF_STATEMENT (LOGIC_EXPRESSION)\n"));
                    }
                    print_syntax_constructions(if_else_statement.bodies[j], if_else_statement.grammars_quantities[j]);
                }
                --global_tab_counter;
                break;
            case DO_WHILE_STATEMENT:
                do_while_statement = *((struct Do_While*)grammars[i].data);
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "DO_WHILE (LOGIC_EXPRESSION)\n"));
                ++global_tab_counter;
                print_syntax_constructions(do_while_statement.body, do_while_statement.grammars_quantity);
                --global_tab_counter;
                break;
            case WHILE_STATEMENT:
                while_statement = *((struct While*)grammars[i].data);
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "WHILE (LOGIC_EXPRESSION)\n"));
                ++global_tab_counter;
                print_syntax_constructions(while_statement.body, while_statement.grammars_quantity);
                --global_tab_counter;
                break;
            case FOR_STATEMENT:
                printf("FOR_STATEMENT\n\n");
                break;
            case FUNCTION_DECLARATION:
                function_declaration = *((struct Function_Declaration*)grammars[i].data);
                printf("%s", concatenate(multiply_string("\t", global_tab_counter), "FUNCTION_DECLARATION\n"));
                ++global_tab_counter;
                print_syntax_constructions(function_declaration.body, function_declaration.quantity_of_grammars);
                --global_tab_counter;
                break;
            case FUNCTION_CALLING:
                printf("FUNCTION_CALLING\n\n");
                break;
            case ASSIGNMENT_EXPRESSION:
                printf("ASSIGNMENT_EXPRESSION\n\n");
                break;
            case GENERAL_ASSIGNMENT_EXPRESSION:
                printf("GENERAL_ASSIGNMENT_EXPRESSION\n\n");
                break;
            case READ_INT:
                printf("READ_INT\n\n");
                break;
            case WRITE_INT:
                printf("WRITE_INT\n\n");
                break;
            case READ_CHAR:
                printf("READ_CHAR\n\n");
                break;
            case WRITE_CHAR:
                printf("WRITE_CHAR\n\n");
                break;
            case UNKNOWN:
                printf("UNKNOWN\n\n");
                break;
        }
    }
}


int main(int argc, char** argv) {
    struct file_handler handler;
    FILE* open_file_pointer;
    FILE* asm_file_pointer;
    char* buffer;

    printf("%s\n", GREETINGS_TEXT);
    printf("%s\n", LOGO);

    if (argc != 3) {
        printf("Incorrect quantity of argument\n");
        return -1;
    }

    open_file_pointer = fopen(argv[1], "r");
    if (open_file_pointer) {
        fseek(open_file_pointer, 0L, SEEK_END);
        handler.size = ftell(open_file_pointer);
        fseek(open_file_pointer, 0L, SEEK_SET);
        buffer = (char*)malloc((handler.size + 1) * sizeof(char));
        fread(buffer, sizeof(char), handler.size, open_file_pointer);
    } else {
        printf("Cannot open file\n");
        return -1;
    }

    // delete_singleline_comments(handler);
    // delete_multiline_comments(handler);
    // fclose(handler.file_pointer);
    // delete_repetitive_spaces(handler);

    struct Lexer* lexer = (struct Lexer*)malloc(sizeof(struct Lexer));
    buffer[strlen(buffer)] = '\0';
    *lexer = init_lexer(buffer, strlen(buffer));
    printf("TOKENS:\n");
    print_all_tokens(get_all_tokens(lexer));
    printf("\n\n\n");

    struct Parser* parser = (struct Parser*)malloc(sizeof(struct Parser));
    *parser = init_parser(lexer);

    int quantity_of_grammars = 0;
    int is_errors_were_occurred = 0;
    struct Grammar* grammars = start_parsing(parser, &quantity_of_grammars, &is_errors_were_occurred);
    printf("\n\nSYNTAX CONSTRUCTIONS:\n");
    print_syntax_constructions(grammars, quantity_of_grammars);
    printf("\n\n\n");

    if (!is_errors_were_occurred) {
        printf("RISC_V ASSEMBLER:\n");
        char* risc_v_assembly = generate_assembly_for_grammars(grammars, quantity_of_grammars);
        printf("%s", risc_v_assembly);

        asm_file_pointer = fopen(argv[2], "w");
        if (asm_file_pointer) {
            fwrite(risc_v_assembly, sizeof(char), strlen(risc_v_assembly), asm_file_pointer);
            fclose(asm_file_pointer);
        } else {
            printf("Cannot open asm file for writing\n");
            return -1;
        }
    }

    return 0;
}
