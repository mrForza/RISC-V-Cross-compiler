#include "parser.h"
#include "unistd.h"
#include "../lexer/lexer.c"


struct Parser init_parser(struct Lexer* lexer) {
    struct Parser parser = {};
    parser.tokens = get_all_tokens(lexer);
    return parser;
}


struct Vector* start_parsing(struct Parser* parser) {
    unsigned long long index = 0;
    void** tokens = parser->tokens->data;
    struct Token* curr_token = (struct Token*)(tokens[index]);
    struct Vector* wirths_grammars = init_vector(256, GRAMMAR);

    while (curr_token->type != END) {
        if (curr_token->type == SEMICOLON) {

        } else {
            if (((struct Token*)(tokens[index - 1]))->type == SEMICOLON) {

            } else {

            }
        }

        curr_token = tokens[index++];
    }

    return wirths_grammars;
}


size_t find_close_bracket_2(void** tokens, size_t start_position, size_t size) {
    size_t counter = 0;
    for (size_t i = start_position; i < size; ++i) {
        if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
            ++counter;
        } else if (((struct Token*)(tokens[i]))->type == RIGHT_ROUND_BRACKET) {
            --counter;
            if (counter == 0) {
                return i;
            }
        }
    }
    return -1;
}


struct Arithmetic_Expression get_arithmetic_expression(void** tokens, size_t start, size_t end) {
    size_t i = start;
    struct Arithmetic_Expression expression;
    bool is_right = false;
    expression.is_left_expr = false;
    expression.is_right_expr = false;

_check_operand:
    if (((struct Token *) (tokens[i]))->type == LEFT_ROUND_BRACKET) {
        size_t close_bracket_pos = find_close_bracket_2(tokens, i, end + 1);

        if (is_right) {
            expression.is_right_expr = true;
            expression.right_operand = (struct Arithmetic_Expression *) malloc(sizeof(struct Arithmetic_Expression));
            *((struct Arithmetic_Expression *) (expression.right_operand)) = get_arithmetic_expression(
                    tokens, i + 1, close_bracket_pos);
        } else {
            expression.is_left_expr = true;
            expression.left_operand = (struct Arithmetic_Expression *) malloc(sizeof(struct Arithmetic_Expression));
            *((struct Arithmetic_Expression *) (expression.left_operand)) = get_arithmetic_expression(
                    tokens, i + 1, close_bracket_pos);
        }

        if (i >= end) {
            return expression;
        }

        i = close_bracket_pos + 1;

    } else {
        if (is_right) {
            expression.right_operand = (char *) malloc(
                    strlen(((struct Token*)(tokens[i]))->attributes->text) * sizeof(char));
            expression.right_operand = ((struct Token*)(tokens[i]))->attributes->text;
        } else {
            expression.left_operand = (char *) malloc(
                    strlen(((struct Token*)(tokens[i]))->attributes->text) * sizeof(char));
            expression.left_operand = ((struct Token*)(tokens[i]))->attributes->text;
        }
        ++i;
    }

    if (i < end) { // !!!!!!!!!!!!!!!!!!!!!
        expression.operator = ((struct Token *) (tokens[i]))->attributes->text;
        ++i;
        is_right = true;
        goto _check_operand;
    }

    return expression;
}
