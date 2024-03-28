#include "complex_structures.h"

size_t find_close_bracket(void** tokens, size_t start_position, size_t size) {
    for (size_t i = start_position; i < size; ++i) {
        if (((struct Token*)(tokens[i]))->type == RIGHT_ROUND_BRACKET) {
            return i;
        }
    }
    return -1;
}


bool is_arithmetic_expression(void** tokens, size_t* start_position, size_t size) {
    unsigned long long i = *start_position;

    _start:
    if (i == size - 1) {
        return true;
    }

    if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
        size_t close_index = find_close_bracket(tokens, i + 1, size);
        if (i == -1) {
            printf("Missing a close bracket in %llu column", ((struct Token*)(tokens[i]))->attributes->column);
            return false;
        }
        size_t new_start_index = i + 1;
        if (!is_arithmetic_expression(tokens, &new_start_index, close_index)) {
            return false;
        }
        i = close_index + 1;
        if (i >= size) {
            return true;
        }
    } else {
        if (((struct Token*)(tokens[i]))->type == DECIMAL_INTEGER_LITERAL
            || ((struct Token*)(tokens[i]))->type == DECIMAL_REAL_LITERAL) {
            if (i == size - 1) {
                return true;
            }
            ++i;
            goto _check_operator;
        }
    }

    _check_operator:
    if (((struct Token*)(tokens[i]))->type == ARITHMETIC_OPERATOR) {
        ++i;
        goto _start;
    }

    return true;
}


bool is_relational_expression(void** tokens, size_t* start_position, size_t size) {
    unsigned long long i = *start_position;

    if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
        size_t close_index = find_close_bracket(tokens, i + 1, size);
        if (i == -1) {
            printf("Missing a close bracket in %llu column", ((struct Token*)(tokens[i]))->attributes->column);
            return false;
        }
        size_t new_start_index = i + 1;
        if (!is_arithmetic_expression(tokens, &new_start_index, close_index)) {
            return false;
        }
        i = close_index + 1;
        if (i >= size) {
            return true;
        }
    } else {
        if (((struct Token*)(tokens[i]))->type == DECIMAL_INTEGER_LITERAL ||
            ((struct Token*)(tokens[i]))->type == DECIMAL_REAL_LITERAL) {
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
        size_t close_index = find_close_bracket(tokens, i + 1, size);
        if (i == -1) {
            printf("Missing a close bracket in", ((struct Token*)(tokens[i]))->attributes->column);
            return false;
        }
        size_t new_start_index = i + 1;
        if (!is_arithmetic_expression(tokens, &new_start_index, close_index)) {
            return false;
        }
        i = close_index + 1;
        if (i >= size) {
            return true;
        }
    } else {
        if (((struct Token*)(tokens[i]))->type == DECIMAL_INTEGER_LITERAL ||
            ((struct Token*)(tokens[i]))->type == DECIMAL_REAL_LITERAL) {
            if (i == size - 1) {
                return true;
            }
            ++i;
        }
    }

    return true;
}


bool is_logical_expression(void** tokens, size_t* start_position, size_t size) {
    unsigned long long i = *start_position;

_start:
    if (i == size - 1) {
        return true;
    }

    if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
        size_t close_index = find_close_bracket(tokens, i + 1, size);
        if (i == -1) {
            printf("Missing a close bracket in %llu column", ((struct Token*)(tokens[i]))->attributes->column);
            return false;
        }
        size_t new_start_index = i + 1;
        size_t temp = i + 1;
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
    }

    return true;
}


bool is_bitwise_expression(void** tokens, size_t* start_position, size_t size) {
    unsigned long long i = *start_position;

    _start:
    if (i == size - 1) {
        return true;
    }

    if (((struct Token*)(tokens[i]))->type == LEFT_ROUND_BRACKET) {
        size_t close_index = find_close_bracket(tokens, i + 1, size);
        if (i == -1) {
            printf("Missing a close bracket in %llu column", ((struct Token*)(tokens[i]))->attributes->column);
            return false;
        }
        size_t new_start_index = i + 1;
        if (!is_arithmetic_expression(tokens, &new_start_index, close_index)
            || !is_bitwise_expression(tokens, &new_start_index, close_index)) {
            return false;
        }
        i = close_index + 1;
        if (i >= size) {
            return true;
        }
    } else {
        if (((struct Token*)(tokens[i]))->type == DECIMAL_INTEGER_LITERAL
            || ((struct Token*)(tokens[i]))->type == DECIMAL_REAL_LITERAL) {
            if (i == size - 1) {
                return true;
            }
            ++i;
            goto _check_operator;
        }
    }

    _check_operator:
    if (((struct Token*)(tokens[i]))->type == BITWISE_OPERATOR) {
        ++i;
        goto _start;
    }

    return true;
}


bool is_single_declaration_expression(void** tokens, size_t* start_position, size_t size) {
    size_t i = *start_position;
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


bool is_single_definition_expression(void** tokens, size_t* start_position, size_t size) {
    size_t i = *start_position;
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
            if (((struct Token*)(tokens[i]))->type == DECIMAL_INTEGER_LITERAL
                || ((struct Token*)(tokens[i]))->type == DECIMAL_REAL_LITERAL
                || ((struct Token*)(tokens[i]))->type == STRING_LITERAL
                || ((struct Token*)(tokens[i]))->type == SYMBOL_LITERAL) {
                return true;
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


bool is_complex_declaration_expression(void** tokens, size_t* start_position, size_t size) {
    size_t i = *start_position;
    if (((struct Token*)(tokens[i]))->type != INT
        && ((struct Token*)(tokens[i]))->type != CHAR
        && ((struct Token*)(tokens[i]))->type != DOUBLE) {
        return false;
    }
    ++i;

    _start:
    if (((struct Token*)(tokens[i]))->type == IDENTIFIER) {
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

    if (((struct Token*)(tokens[i]))->type == MULTIPLICATION_SIGN) {
        ++i;
        goto _start;
    }
    return false;
}


bool is_complex_definition_expression(void** tokens, size_t* start_position, size_t size) {
    size_t i = *start_position;
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
            if (((struct Token*)(tokens[i]))->type == DECIMAL_INTEGER_LITERAL
                || ((struct Token*)(tokens[i]))->type == DECIMAL_REAL_LITERAL
                || ((struct Token*)(tokens[i]))->type == STRING_LITERAL
                || ((struct Token*)(tokens[i]))->type == SYMBOL_LITERAL) {
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