#include "syntax_analyzer.h"
#include "../lexer/token.h"
#include "stdio.h"


int find_close_bracket(void** tokens, int start_position, int size) {
    int counter = 0;
    for (int i = start_position; i < size; ++i) {
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


int find_close_curly_bracket(void** tokens, int start_position, int size) {
    int counter = 0;
    for (int i = start_position; i <= size; ++i) { // !!!!!!!!!!!!!!!!!!!!
        if (((struct Token*)(tokens[i]))->type == LEFT_CURLY_BRACKET) {
            ++counter;
        } else if (((struct Token*)(tokens[i]))->type == RIGHT_CURLY_BRACKET) {
            --counter;
            if (counter == 0) {
                return i;
            }
        }
    }
    return -1;
}


int find_nearest_semicolon(void** tokens, int start_position, int size) {
    for (int i = start_position; i < size; ++i) {
        if (((struct Token*)(tokens[i]))->type == SEMICOLON) {
            return i;
        }
    }
    return -1;
}


bool is_arithmetic_expression(void** tokens, const int* start_position, int size) {
    int i = *start_position;

    _start:
    if (i >= size - 1) {
        return true;
    }

    if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
        int close_index = find_close_bracket(tokens, i, size + 1);
        if (i == -1) {
            printf("Missing a close bracket in %llu column", ((struct Token*)(tokens[i]))->attributes->column);
            return false;
        }
        int new_start_index = i + 1;
        if (!is_arithmetic_expression(tokens, &new_start_index, close_index)) {
            return false;
        }
        i = close_index + 1;
        if (i >= size) {
            return true;
        }
    } else {
        if (((struct Token*)(tokens[i]))->type == DOUBLE_LITERAL
            || ((struct Token*)(tokens[i]))->type == DECIMAL_INT_LITERAL
            || ((struct Token*)(tokens[i]))->type == IDENTIFIER) {
            if (i == size - 1) {
                return true;
            }
            ++i;
            goto _check_operator;
        }
    }

    _check_operator:
    if (((struct Token*)(tokens[i]))->type == ARITHMETIC_OPERATOR ||
        ((struct Token*)(tokens[i]))->type == BITWISE_OPERATOR) {
        if (((struct Token*)(tokens[i]))->type == BITWISE_OPERATOR &&
            (((struct Token*)(tokens[i - 1]))->type == DOUBLE_LITERAL ||
             ((struct Token*)(tokens[i + 1]))->type == DOUBLE_LITERAL)) {
            printf("SEMANTIC ERROR: Incorrect literals in bitwise expression!");
            return false;
        }
        ++i;
        goto _start;
    } else { // !!!!!!!!!!!!!!!
        return false;
    }
}


bool is_relational_expression(void** tokens, const int* start_position, int size) {
    int i = *start_position;

    if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
        int close_index = find_close_bracket(tokens, i + 1, size);
        if (i == -1) {
            printf("Missing a close bracket in %llu column", ((struct Token*)(tokens[i]))->attributes->column);
            return false;
        }
        int new_start_index = i + 1;
        if (!is_arithmetic_expression(tokens, &new_start_index, close_index)) {
            return false;
        }
        i = close_index + 1;
        if (i >= size) {
            return true;
        }
    } else {
        if (((struct Token*)(tokens[i]))->type == DOUBLE_LITERAL ||
            ((struct Token*)(tokens[i]))->type == DECIMAL_INT_LITERAL ||
            ((struct Token*)(tokens[i]))->type == IDENTIFIER) {
            if (i == size - 1) {
                return true;
            }
            ++i;
        }
    }

    if (((struct Token*)(tokens[i]))->type == COMPARISON_OPERATOR) {
        ++i;
    }

    if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
        int close_index = find_close_bracket(tokens, i + 1, size);
        if (i == -1) {
            printf("Missing a close bracket in %llu column", ((struct Token*)(tokens[i]))->attributes->column);
            return false;
        }
        int new_start_index = i + 1;
        if (!is_arithmetic_expression(tokens, &new_start_index, close_index)) {
            return false;
        }
        i = close_index + 1;
        if (i >= size) {
            return true;
        }
    } else {
        if (((struct Token*)(tokens[i]))->type == DOUBLE_LITERAL || // ?????
            ((struct Token*)(tokens[i]))->type == DECIMAL_INT_LITERAL ||
            ((struct Token*)(tokens[i]))->type == IDENTIFIER) {
            if (i == size - 1) {
                return true;
            }
            ++i;
        }
    }

    return true;
}


bool is_logical_expression(void** tokens, const int* start_position, int size) {
    int i = *start_position;

    _start:
    if (i == size - 1) {
        return true;
    }

    if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
        int close_index = find_close_bracket(tokens, i + 1, size);
        if (i == -1) {
            printf("Missing a close bracket in %llu column", ((struct Token*)(tokens[i]))->attributes->column);
            return false;
        }
        int new_start_index = i + 1;
        int temp = i + 1;
        if (!is_logical_expression(tokens, &new_start_index, close_index)
            || !is_relational_expression(tokens, &temp, close_index)) {
            return false;
        }
        i = close_index + 1;
        if (i >= size) {
            return true;
        }
    } else {
        if (((struct Token*)(tokens[i]))->type == IDENTIFIER) {
            if (i == size - 1) {
                return true;
            }
            ++i;
            goto _check_operator;
        }
    }

    _check_operator:
    if (((struct Token*)(tokens[i]))->type == LOGIC_OPERATOR) {
        ++i;
        goto _start;
    } else {
        return false;
    }

    return true;
}


bool is_single_declaration_expression(void** tokens, const int* start_position, int size) {
    int i = *start_position;
    if (((struct Token*)(tokens[i]))->type != INT
        && ((struct Token*)(tokens[i]))->type != CHAR
        && ((struct Token*)(tokens[i]))->type != DOUBLE) {
        return false;
    }
    ++i;

    if (((struct Token*)(tokens[i]))->type == IDENTIFIER) {
        return true;
    }
    if (((struct Token*)(tokens[i]))->type == MULTIPLICATION_SIGN) {
        ++i;
        if (((struct Token*)(tokens[i]))->type == IDENTIFIER) {
            return true;
        }
    }
    return false;
}


bool is_single_definition_expression(void** tokens, const int* start_position, int size) {
    int i = *start_position;
    if (((struct Token*)(tokens[i]))->type != INT
        && ((struct Token*)(tokens[i]))->type != CHAR
        && ((struct Token*)(tokens[i]))->type != DOUBLE) {
        return false;
    }
    ++i;

_additional_check:
    if ((((struct Token*)(tokens[i - 1]))->type == INT ||
         ((struct Token*)(tokens[i - 1]))->type == CHAR) &&
        ((struct Token*)(tokens[i + 2]))->type == DOUBLE_LITERAL) {
        printf("SEMANTIC ERROR: Cannot pass a double value into an int var");
        return false;
    }

_start:
    if (((struct Token*)(tokens[i]))->type == IDENTIFIER) {
        ++i;
        if (((struct Token*)(tokens[i]))->type == ASSIGN_OPERATOR) {
            ++i;
            if (is_arithmetic_expression(tokens, &i, size)) {
                return true;
            } else {
                if (((struct Token*)(tokens[i]))->type == DOUBLE_LITERAL
                    || ((struct Token*)(tokens[i]))->type == DECIMAL_INT_LITERAL
                    || ((struct Token*)(tokens[i]))->type == STRING_LITERAL
                    || ((struct Token*)(tokens[i]))->type == CHAR_LITERAL
                    || ((struct Token*)(tokens[i]))->type == IDENTIFIER) {
                    return true;
                }
            }

            return false;
        }
    }

    if (((struct Token*)(tokens[i]))->type == MULTIPLICATION_SIGN) {
        ++i;
        goto _start;
    }
    return false;
}


bool is_complex_declaration_expression(void** tokens, const int* start_position, int size) {
    int i = *start_position;
    if (((struct Token*)(tokens[i]))->type != INT
        && ((struct Token*)(tokens[i]))->type != CHAR
        && ((struct Token*)(tokens[i]))->type != DOUBLE) {
        return false;
    }
    ++i;

    _start:
    if (((struct Token*)(tokens[i]))->type == IDENTIFIER) {
        if (i >= size - 1) {
            return true;
        }
        ++i;
        if (((struct Token*)(tokens[i]))->type == COMMA) {
            ++i;
            goto _start;
        }
        return true;
    }

    if (((struct Token*)(tokens[i]))->type == MULTIPLICATION_SIGN) {
        ++i;
        goto _start;
    }
    return false;
}


bool is_complex_definition_expression(void** tokens, const int* start_position, int size) {
    int i = *start_position;
    if (((struct Token*)(tokens[i]))->type != INT
        && ((struct Token*)(tokens[i]))->type != CHAR
        && ((struct Token*)(tokens[i]))->type != DOUBLE) {
        return false;
    }
    ++i;

    _start:
    if (((struct Token*)(tokens[i]))->type == IDENTIFIER) {
        ++i;
        if (((struct Token*)(tokens[i]))->type == ASSIGN_OPERATOR) {
            ++i;
            if (((struct Token*)(tokens[i]))->type == DOUBLE_LITERAL
                || ((struct Token*)(tokens[i]))->type == DECIMAL_INT_LITERAL
                || ((struct Token*)(tokens[i]))->type == STRING_LITERAL
                || ((struct Token*)(tokens[i]))->type == CHAR_LITERAL
               || ((struct Token*)(tokens[i]))->type == IDENTIFIER) {
                if (i == size - 1) {
                    return true;
                }
                ++i;
                if (((struct Token*)(tokens[i]))->type == COMMA) {
                    ++i;
                    goto _start;
                }
                return true;
            }
        }
    }

    if (((struct Token*)(tokens[i]))->type == MULTIPLICATION_SIGN) {
        ++i;
        goto _start;
    }
    return false;
}


bool is_prefix_expression(void** tokens, const int* start_position, int size) {
    return true;
}


bool is_postfix_expression(void** tokens, const int* start_position, int size) {
    return true;
}


bool is_if_else_statement(void** tokens, const int* start_position, int size) {
    int i = *start_position;

    if (((struct Token*)(tokens[i]))->type == IF) {
        ++i;
        _check_condition_expression:
        if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
            int closed_bracket_index = find_close_bracket(tokens, i, size);
            int temp = i + 1;
            if (is_logical_expression(tokens, &temp, closed_bracket_index - 1)
                || is_relational_expression(tokens, &temp, closed_bracket_index - 1)) {
                i = closed_bracket_index + 1;
                if (((struct Token*)(tokens[i]))->type == LEFT_CURLY_BRACKET) {
                    int closed_curly_bracket_index = find_close_curly_bracket(tokens, i, size);
                    i = closed_curly_bracket_index + 1;
                    if (closed_curly_bracket_index != -1 && closed_curly_bracket_index <= size) {
                        return true;
                    }
                    goto _check_another_branches;
                } else {
                    printf("SYNTAX ERROR: Missing curly bracket!");
                    return false;
                }
            } else {
                printf("SYNTAX ERROR: Incorrect conditional or logic expression!");
                return false;
            }
        } else{
            printf("SYNTAX ERROR: No left round bracket in condition!");
            return false;
        }
    }

    _check_another_branches:
    if (((struct Token*)(tokens[i]))->type == ELSE) {
        ++i;
        if (((struct Token*)(tokens[i]))->type == IF) {
            ++i;
            goto _check_condition_expression;
        } else {
            if (((struct Token*)(tokens[i]))->type == LEFT_CURLY_BRACKET) {
                int closed_curly_bracket_index = find_close_curly_bracket(tokens, i, size);
                i = closed_curly_bracket_index + 1;
                if (closed_curly_bracket_index != -1 && closed_curly_bracket_index <= size) {
                    return true;
                }
            } else {
                printf("SYNTAX ERROR: Missing curly bracket!");
                return false;
            }
        }
    }

    return false;
}


bool is_while_statement(void** tokens, const int* start_position, int size) {
    int i = *start_position;

    if (((struct Token*)(tokens[i]))->type == WHILE) {
        ++i;
        if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
            int closed_bracket_index = find_close_bracket(tokens, i, size);
            int temp = i + 1;
            if (is_relational_expression(tokens, &temp, closed_bracket_index - 1)) {
                i = closed_bracket_index + 1;
                if (((struct Token*)(tokens[i]))->type == LEFT_CURLY_BRACKET) {
                    int closed_curly_bracket_index = find_close_curly_bracket(tokens, i, size);
                    if (closed_curly_bracket_index != -1) {
                        return true;
                    }
                    printf("\nSYNTAX ERROR IN COLUMN %llu: Missing curly bracket!",
                           (((struct Token*)(tokens[i]))->attributes->column));
                    return false;
                } else {
                    printf("\nSYNTAX ERROR IN COLUMN %llu Missing curly bracket!",
                           (((struct Token*)(tokens[i]))->attributes->column));
                    return false;
                }
            } else {
                printf("\nSYNTAX ERROR IN COLUMN %llu: Incorrect conditional expression!", i);
                return false;
            }
        } else {
            printf("\nSYNTAX ERROR IN COLUMN %llu: No left round bracket in condition!", i);
            return false;
        }
    }

    return false;
}


bool is_do_while_statement(void** tokens, const int* start_position, int size) {
    int i = *start_position;

    if (((struct Token*)(tokens[i]))->type == DO) {
        ++i;
        if (((struct Token*)(tokens[i]))->type == LEFT_CURLY_BRACKET) {
            int closed_curly_bracket_index = find_close_curly_bracket(tokens, i, size);
            if (closed_curly_bracket_index == -1) {
                printf("ERROR: Missing curly bracket!");
                return false;
            }
            i = closed_curly_bracket_index + 1;
            goto _check_while;
        } else {
            printf("ERROR: Missing curly bracket!");
            return false;
        }
    } else {
        return false;
    }

    _check_while:
    if (((struct Token*)(tokens[i]))->type == WHILE) {
        ++i;
        if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
            int closed_bracket_index = find_close_bracket(tokens, i, size);
            int temp = i + 1;
            if (closed_bracket_index == -1) {
                printf("SYNTAX ERROR: Missing closed round bracket!");
            }
            if (is_relational_expression(tokens, &temp, closed_bracket_index - 1)) {
                i = closed_bracket_index + 2;
                return true;
            } else {
                printf("ERROR: Incorrect conditional expression!");
                return false;
            }
        } else {
            printf("ERROR: No left round bracket in condition!");
            return false;
        }
    }

    return false;
}


bool is_for_statement(void** tokens, const int* start_position, int size) {
    int i = *start_position;

    if (((struct Token*)(tokens[i]))->type == FOR) {
        ++i;
        if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
            int closed_bracket_index = find_close_bracket(tokens, i, size);
            int temp = i + 1;
            int semicolon_index = find_nearest_semicolon(tokens, i, closed_bracket_index);
            if (is_single_definition_expression(tokens, &temp, semicolon_index - 1)) {
                i = semicolon_index + 1;
                semicolon_index = find_nearest_semicolon(tokens, i, closed_bracket_index);
                if (is_relational_expression(tokens, &i, semicolon_index - 1)) {
                    i = semicolon_index + 1;
                    if (is_prefix_expression(tokens, &i, closed_bracket_index - 1) ||
                        is_postfix_expression(tokens, &i, closed_bracket_index - 1)) {
                        i = closed_bracket_index + 1;
                        goto _check_curly_brackets;
                    }
                    printf("ERROR: No expression in for!");
                    return false;
                } else {
                    printf("ERROR: No condition in for!");
                    return false;
                }
            } else {
                printf("ERROR: No var initialization in for!");
                return false;
            }
        } else {
            printf("ERROR: No left round bracket in condition!");
            return false;
        }
    }

    _check_curly_brackets:
    if (((struct Token*)(tokens[i]))->type == LEFT_CURLY_BRACKET) {
        int closed_curly_bracket_index = find_close_curly_bracket(tokens, i, size + 1);
        if (closed_curly_bracket_index != -1) {
            return true;
        }
        printf("ERROR: Missing curly bracket!");
        return false;
    }

    printf("ERROR: Missing curly bracket!");
    return false;
}


bool is_assignment_expression(void** tokens, const int* start_position, int size) {
    int i = *start_position;

    if (((struct Token*)(tokens[i]))->type == IDENTIFIER) {
        ++i;
        if (((struct Token*)(tokens[i]))->type == ARITHMETIC_ASSIGN_OPERATOR) {
            ++i;
            if (is_arithmetic_expression(tokens, &i, size)) {
                return true;
            } else {
                if (((struct Token*)(tokens[i]))->type == DECIMAL_INT_LITERAL ||
                    ((struct Token*)(tokens[i]))->type == CHAR_LITERAL) {
                    return true;
                }
            }
        }
    }
    return false;
}