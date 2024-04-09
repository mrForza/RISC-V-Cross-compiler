#include "stdio.h"
#include "include/file_handler/file_validator.c"
#include "include/file_handler/file_handler.c"
#include "include/preprocessor/preprocessor.c"
#include "include/parser/parser.c"

const char* GREETINGS_TEXT = "The simple Small-C preprocessor\n";

const char* LOGO = "\n"
                   "  ____  ___ ____   ____   __     __   ____                          ____                      _ _           \n"
                   " |  _ \\|_ _/ ___| / ___|  \\ \\   / /  / ___|_ __ ___  ___ ___       / ___|___  _ __ ___  _ __ (_) | ___ _ __ \n"
                   " | |_) || |\\___ \\| |   ____\\ \\ / /  | |   | '__/ _ \\/ __/ __|_____| |   / _ \\| '_ ` _ \\| '_ \\| | |/ _ \\ '__|\n"
                   " |  _ < | | ___) | |__|_____\\ V /   | |___| | | (_) \\__ \\__ \\_____| |__| (_) | | | | | | |_) | | |  __/ |   \n"
                   " |_| \\_\\___|____/ \\____|     \\_/     \\____|_|  \\___/|___/___/      \\____\\___/|_| |_| |_| .__/|_|_|\\___|_|   \n"
                   "                                                                                       |_|                  ";

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
        switch (grammars[i].type) {
            case SINGLE_DECLARATION:
                printf("SINGLE_DECLARATION\n");
                break;
            case COMPLEX_DECLARATION:
                printf("COMPLEX_DECLARATION\n");
                break;
            case SINGLE_DEFINITION:
                printf("SINGLE_DEFINITION\n");
                break;
            case COMPLEX_DEFINITION:
                printf("COMPLEX_DEFINITION\n");
                break;
            case ARITHMETIC_EXPRESSION:
                printf("ARITHMETIC_EXPRESSION\n");
                break;
            case LOGIC_EXPRESSION:
                printf("LOGIC_EXPRESSION\n");
                break;
            case RELATIONAL_EXPRESSION:
                printf("RELATIONAL_EXPRESSION\n");
                break;
            case IF_ELSE_STATEMENT:

                printf("IF_ELSE_STATEMENT\n");
                break;
            case DO_WHILE_STATEMENT:
                printf("DO_WHILE_STATEMENT\n");
                break;
            case WHILE_STATEMENT:
                printf("WHILE_STATEMENT\n");
                break;
            case FOR_STATEMENT:
                printf("FOR_STATEMENT\n");
                break;
            case FUNCTION_DECLARATION:
                printf("FUNCTION_DECLARATION\n");
                break;
        }
    }
}


int main() {
    printf("%s", GREETINGS_TEXT);
    printf("%s", LOGO);

    struct file_handler handler;
    FILE* open_file_pointer;
    /*
    while (true) {
        char* file_name = get_full_name();
        if (validate_filename(file_name)) {
            char** splitted_info = split_full_name(file_name);
            char* name = splitted_info[0];
            char* extension = splitted_info[1];
            open_file_pointer = fopen(file_name, "r");
            if (open_file_pointer) {
                if (validate_file_existance(open_file_pointer)) {
                    handler.file_pointer = open_file_pointer;
                    handler.name = name;
                    handler.extension = extension;
                    fseek(open_file_pointer, 0L, SEEK_END);
                    handler.size = ftell(open_file_pointer);
                    fseek(open_file_pointer, 0L, SEEK_SET);
                    buffer = (char*)malloc(handler.size * sizeof(char));
                    fread(buffer, sizeof(char), handler.size, open_file_pointer);
                    break;
                }
            }
        }
    }
    printf("Name: %s\nExtension: .%s\nSize: %llu bytes\n\n", handler.name, handler.extension, handler.size);
    delete_singleline_comments(handler);
    delete_multiline_comments(handler);
    fclose(handler.file_pointer);
    delete_repetitive_spaces(handler)*/

    open_file_pointer = fopen("testfile.c", "r");
    fseek(open_file_pointer, 0L, SEEK_END);
    size_t size = ftell(open_file_pointer);
    fseek(open_file_pointer, 0L, SEEK_SET);
    char* buffer = (char*)malloc(sizeof(char) * size);
    fread(buffer, sizeof(char), size, open_file_pointer);

    struct Lexer* lexer = (struct Lexer*)malloc(sizeof(struct Lexer));
    *lexer = init_lexer(buffer, strlen(buffer));
    print_all_tokens(get_all_tokens(lexer));
    printf("\n\n");

    struct Parser* parser = (struct Parser*)malloc(sizeof(struct Parser));
    *parser = init_parser(lexer);

    int quantity_of_grammars = 0;
    struct Grammar* grammars = start_parsing(parser, &quantity_of_grammars);
    print_syntax_constructions(grammars, quantity_of_grammars);
    return 0;
}
