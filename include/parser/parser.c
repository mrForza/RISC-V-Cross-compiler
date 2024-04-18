#include "parser.h"
#include "syntax_analyzer.c"
#include "complex_structures.c"


struct Parser init_parser(struct Lexer* lexer) {
    struct Parser parser = {};
    parser.tokens = get_all_tokens(lexer);
    return parser;
}


bool check_main(void** tokens, int size) {
    if (((struct Token*)(tokens[0]))->type == INT &&
        ((struct Token*)(tokens[1]))->type == IDENTIFIER &&
        strcmp(((struct Token*)(tokens[1]))->attributes->text, "main") == 0 &&
        ((struct Token*)(tokens[2]))->type == LEFT_ROUND_BRACKET &&
        ((struct Token*)(tokens[3]))->type == RIGHT_ROUND_BRACKET &&
        ((struct Token*)(tokens[4]))->type == LEFT_CURLY_BRACKET &&
        ((struct Token*)(tokens[size - 2]))->type == RIGHT_CURLY_BRACKET) {
        return true;
    }
    return false;
}


int find_semicolon_position(void** tokens, int start, int size) {
    for (int i = start; i <= size; ++i) {
        if (((struct Token *) (tokens[i]))->type == SEMICOLON) {
            return i;
        }
    }
    return -1;
}


int find_close_bracket_2(void** tokens, int start_position, int size) {
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


int get_last_syntax_position(void** tokens, int start, int end, int* is_error) {
    int i = start;
    int round_close_index, curly_close_index;
    if (((struct Token*)(tokens[i]))->type == IF) {
    _check_if_else:
        round_close_index = find_close_bracket_2(tokens, i, end);
        if (round_close_index == -1) {
            printf("SYNTAX ERROR: Missing curly bracket");
            *is_error = 1;
            return -1;
        }
        i = round_close_index + 1;
        curly_close_index = find_close_curly_bracket(tokens, i, end);
        if (curly_close_index == -1) {
            printf("SYNTAX ERROR: Missing curly bracket");
            *is_error = 1;
            return -1;
        }
        i = curly_close_index + 1;
        if (((struct Token*)(tokens[i]))->type == ELSE) {
            if (((struct Token*)(tokens[i + 1]))->type == IF) {
                i += 2;
                goto _check_if_else;
            }
            ++i;
            curly_close_index = find_close_curly_bracket(tokens, i, end);
            if (curly_close_index == -1) {
                printf("SYNTAX ERROR: Missing curly bracket");
                *is_error = 1;
                return -1;
            }
            i = curly_close_index;
        }
        return i;
    } else if (((struct Token*)(tokens[i]))->type == DO) {
        int curly_bracket_index = find_close_curly_bracket(tokens, i + 1, end);
        i = curly_bracket_index + 1;
        if (((struct Token*)(tokens[i]))->type == WHILE) {
            ++i;
            int round_bracket_index = find_close_bracket(tokens, i, end);
            if (round_bracket_index == -1) {
                printf("SYNTAX ERROR: Missing round bracket");
                *is_error = 1;
                return -1;
            }
            i = round_bracket_index + 1;
            return i;
        } else {
            printf("SYNTAX ERROR: Missing while keyword in the loop");
            *is_error = 1;
            return -1;
        }

    } else if (((struct Token*)(tokens[i]))->type == WHILE) {
        int round_bracket_index = find_close_bracket(tokens, i + 1, end);
        if (round_bracket_index == -1) {
            printf("SYNTAX ERROR: Missing round bracket");
            *is_error = 1;
            return -1;
        }
        i = round_bracket_index + 1;
        int curly_bracket_index = find_close_curly_bracket(tokens, i, end);
        if (curly_bracket_index == -1) {
            printf("SYNTAX ERROR: Missing curly bracket");
            *is_error = 1;
            return -1;
        }
        i = curly_bracket_index;
        return i;
    } else if (((struct Token*)(tokens[i]))->type == FOR) {
        int round_bracket_index = find_close_bracket(tokens, i + 1, end);
        if (round_bracket_index == -1) {
            printf("SYNTAX ERROR: Missing round bracket");
            *is_error = 1;
            return -1;
        }
        i = round_bracket_index + 1;
        int curly_bracket_index = find_close_curly_bracket(tokens, i + 1, end);
        if (curly_bracket_index == -1) {
            printf("SYNTAX ERROR: Missing curly bracket");
            *is_error = 1;
            return -1;
        }
        i = curly_bracket_index;
        return i;
    }

    printf("SEMANTIC ERROR: Cannot parse syntax construction");
    *is_error = 1;
    return -1;
}


int get_quantity_of_variables(struct Grammar* grammars, int size) {
    int quantity_of_variables = 0;
    for (int i = 0; i < size; ++i) {
        if (grammars[i].type == SINGLE_DEFINITION) {
            ++quantity_of_variables;
        } else if (grammars[i].type == COMPLEX_DEFINITION) {
            quantity_of_variables += ((struct Complex_Definition*)(grammars[i].data))->quantity_of_variables;
        }
    }

    return quantity_of_variables;
}


struct Grammar* get_intermediate_representation_for_scope(
        void** tokens, int start, int end, int* quantity_of_grammars, int* is_error) {
    int i = start;
    struct Grammar* grammars = malloc(1024 * sizeof(struct Grammar*));
    int grammars_index = 0;
    while (i < end - 1) {
        ++(*quantity_of_grammars);
        struct Grammar grammar;
        if (((struct Token*)(tokens[i]))->type == IF ||
            ((struct Token*)(tokens[i]))->type == WHILE ||
            ((struct Token*)(tokens[i]))->type == DO ||
            ((struct Token*)(tokens[i]))->type == FOR) {
            int end_index = get_last_syntax_position(tokens, i, end, is_error);
            if (*is_error) {
                struct Unknown* unknown_grammar = (struct Unknown*)malloc(sizeof(struct Unknown));
                grammar.data = unknown_grammar;
                grammars[grammars_index++] = grammar;
                return grammars;
            }
            if (is_if_else_statement(tokens, &i, end_index, is_error)) {
                struct If_Else* expression
                        = (struct If_Else*)malloc(sizeof(struct If_Else));
                *expression = get_if_else_statement(tokens, i, end_index, is_error);
                grammar.data = expression;
                grammar.type = IF_ELSE_STATEMENT;
                grammars[grammars_index++] = grammar;
            } else if (is_do_while_statement(tokens, &i, end_index, is_error)) {
                struct Do_While* expression
                        = (struct Do_While*)malloc(sizeof(struct Do_While));
                *expression = get_do_while_statement(tokens, i, end_index, is_error);
                grammar.data = expression;
                grammar.type = DO_WHILE_STATEMENT;
                grammars[grammars_index++] = grammar;
            } else if (is_while_statement(tokens, &i, end_index, is_error)) {
                struct While* expression
                        = (struct While*)malloc(sizeof(struct While));
                *expression = get_while_statement(tokens, i, end_index, is_error);
                grammar.data = expression;
                grammar.type = WHILE_STATEMENT;
                grammars[grammars_index++] = grammar;
            } else if (is_for_statement(tokens, &i, end_index, is_error)) {
                struct For* expression
                        = (struct For*)malloc(sizeof(struct For));
                *expression = get_for_statement(tokens, i, end_index, is_error);
                grammar.data = expression;
                grammar.type = FOR_STATEMENT;
                grammars[grammars_index++] = grammar;
            } else {
                struct Unknown* unknown_grammar = (struct Unknown*)malloc(sizeof(struct Unknown));
                grammar.data = unknown_grammar;
            }
            i = end_index + 1;
        } else {
            int semicolon_position = find_semicolon_position(tokens, i, end);
            if (is_general_assignment_expression(tokens, &i, semicolon_position - 1, is_error)) {
                struct General_Assignment_Expression* expression
                        = (struct General_Assignment_Expression*)malloc(sizeof(struct General_Assignment_Expression));
                *expression = get_general_assignment_expression(tokens, i, semicolon_position - 1, is_error);
                grammar.data = expression;
                grammar.type = GENERAL_ASSIGNMENT_EXPRESSION;
                grammars[grammars_index++] = grammar;
            }
            if (is_assignment_expression(tokens, &i, semicolon_position - 1, is_error)) {
                struct Assignment_Expression* expression
                        = (struct Assignment_Expression*)malloc(sizeof(struct Assignment_Expression));
                *expression = get_assignment_expression(tokens, i, semicolon_position - 1, is_error);
                grammar.data = expression;
                grammar.type = ASSIGNMENT_EXPRESSION;
                grammars[grammars_index++] = grammar;
            } else if (is_single_definition_expression(tokens, &i, semicolon_position - 1, is_error)) {
                struct Single_Definition* expression
                        = (struct Single_Definition*)malloc(sizeof(struct Single_Definition));
                *expression = get_single_definition(tokens, i, semicolon_position - 1, is_error);
                grammar.data = expression;
                grammar.type = SINGLE_DEFINITION;
                grammars[grammars_index++] = grammar;
            } else if (is_complex_declaration_expression(tokens, &i, semicolon_position - 1, is_error)) {
                struct Complex_Declaration* expression
                        = (struct Complex_Declaration*)malloc(sizeof(struct Complex_Declaration));
                *expression = get_complex_declaration(tokens, i, semicolon_position - 1, is_error);
                grammar.data = expression;
                grammar.type = COMPLEX_DECLARATION;
                grammars[grammars_index++] = grammar;
            } else if (is_single_declaration_expression(tokens, &i, semicolon_position - 1, is_error)) {
                struct Single_Declaration* expression
                        = (struct Single_Declaration*)malloc(sizeof(struct Single_Declaration));
                *expression = get_single_declaration(tokens, i, semicolon_position - 1, is_error);
                grammar.data = expression;
                grammar.type = SINGLE_DECLARATION;
                grammars[grammars_index++] = grammar;
            } else if (is_arithmetic_expression(tokens, &i, semicolon_position - 1, is_error)) {
                struct Arithmetic_Expression* expression
                        = (struct Arithmetic_Expression*)malloc(sizeof(struct Arithmetic_Expression));
                *expression = get_arithmetic_expression(tokens, i, semicolon_position - 1, is_error);
                grammar.data = expression;
                grammar.type = ARITHMETIC_EXPRESSION;
                grammars[grammars_index++] = grammar;
            } else if (is_logical_expression(tokens, &i, semicolon_position - 1, is_error)) {
                struct Logic_Expression* expression
                        = (struct Logic_Expression*)malloc(sizeof(struct Logic_Expression));
                *expression = get_logic_expression(tokens, i, semicolon_position - 1, is_error);
                grammar.data = expression;
                grammar.type = LOGIC_EXPRESSION;
                grammars[grammars_index++] = grammar;
            } else if (is_relational_expression(tokens, &i, semicolon_position - 1, is_error)) {
                struct Relational_Expression* expression
                        = (struct Relational_Expression*)malloc(sizeof(struct Relational_Expression));
                *expression = get_relational_expression(tokens, i, semicolon_position - 1, is_error);
                grammar.data = expression;
                grammar.type = RELATIONAL_EXPRESSION;
                grammars[grammars_index++] = grammar;
            } else if (is_read_int(tokens, &i, semicolon_position - 1, is_error)) {
                struct Read_Int_Function* read_int
                        = (struct Read_Int_Function*)malloc(sizeof(struct Read_Int_Function));
                *read_int = get_read_int(tokens, i, semicolon_position - 1, is_error);
                grammar.data = read_int;
                grammar.type = READ_INT;
                grammars[grammars_index++] = grammar;
            } else if (is_write_int(tokens, &i, semicolon_position - 1, is_error)) {
                struct Write_Int_Function* write_int
                        = (struct Write_Int_Function*)malloc(sizeof(struct Write_Int_Function));
                *write_int = get_write_int(tokens, i, semicolon_position - 1, is_error);
                grammar.data = write_int;
                grammar.type = WRITE_INT;
                grammars[grammars_index++] = grammar;
            } else {
                struct Unknown* unknown_grammar = (struct Unknown*)malloc(sizeof(struct Unknown));
                grammar.data = unknown_grammar;
            }
            if (i == -1) {
                break;
            }
            i = semicolon_position + 1;
        }
    }

    return grammars;
}


struct Grammar* start_parsing(struct Parser* parser, int* quantity_of_grammars, int* is_error) {
    void** tokens = parser->tokens->data;
    struct Grammar* grammars = malloc(128 * sizeof(void*));

    if (check_main(tokens, parser->tokens->size) == true) {
        int index = 5;
        grammars = get_intermediate_representation_for_scope(
                tokens, index, parser->tokens->size - 2, quantity_of_grammars, is_error);
        if (*is_error) {
            return grammars;
        }
    } else {
        printf("SEMANTIC ERROR: No main function in your file!");
    }

    return grammars;
}


struct Arithmetic_Expression get_arithmetic_expression(void** tokens, int start, int end, int* is_error) {
    int i = start;
    struct Arithmetic_Expression expression;
    bool is_right = false;
    expression.is_left_expr = false;
    expression.is_right_expr = false;

_check_operand:
    if (((struct Token *) (tokens[i]))->type == LEFT_ROUND_BRACKET) {
        int close_bracket_pos = find_close_bracket_2(tokens, i, end + 1);

        if (is_right) {
            expression.is_right_expr = true;
            expression.right_operand = (struct Arithmetic_Expression *) malloc(sizeof(struct Arithmetic_Expression));
            *((struct Arithmetic_Expression *) (expression.right_operand)) = get_arithmetic_expression(
                    tokens, i + 1, close_bracket_pos, is_error);
        } else {
            expression.is_left_expr = true;
            expression.left_operand = (struct Arithmetic_Expression *) malloc(sizeof(struct Arithmetic_Expression));
            *((struct Arithmetic_Expression *) (expression.left_operand)) = get_arithmetic_expression(
                    tokens, i + 1, close_bracket_pos, is_error);
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


struct Relational_Expression get_relational_expression(void** tokens, int start, int end, int* is_error) {
    int i = start;
    struct Relational_Expression expression;
    bool is_right = false;
    expression.is_left_expr = false;
    expression.is_right_expr = false;

    _check_operand:
    if (((struct Token *) (tokens[i]))->type == LEFT_ROUND_BRACKET) {
        int close_bracket_pos = find_close_bracket_2(tokens, i, end + 1);

        if (is_right) {
            expression.is_right_expr = true;
            expression.right_operand = (struct Arithmetic_Expression *) malloc(sizeof(struct Arithmetic_Expression));
            *((struct Arithmetic_Expression *) (expression.right_operand)) = get_arithmetic_expression(
                    tokens, i + 1, close_bracket_pos, is_error);
        } else {
            expression.is_left_expr = true;
            expression.left_operand = (struct Arithmetic_Expression *) malloc(sizeof(struct Arithmetic_Expression));
            *((struct Arithmetic_Expression *) (expression.left_operand)) = get_arithmetic_expression(
                    tokens, i + 1, close_bracket_pos, is_error);
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
        expression.operator = ((struct Token *)(tokens[i]))->attributes->text;
        ++i;
        is_right = true;
        goto _check_operand;
    }

    return expression;
}


struct Logic_Expression get_logic_expression(void** tokens, int start, int end, int* is_error) {
    int i = start;
    struct Logic_Expression expression;
    bool is_right = false;
    expression.is_left_expr = false;
    expression.is_right_expr = false;

    _check_operand:
    if (((struct Token *) (tokens[i]))->type == LEFT_ROUND_BRACKET) {
        int close_bracket_pos = find_close_bracket_2(tokens, i, end + 1);

        if (is_right) {
            expression.is_right_expr = true;
            expression.right_operand = (struct Relational_Expression *) malloc(sizeof(struct Relational_Expression));
            *((struct Relational_Expression *) (expression.right_operand)) = get_relational_expression(
                    tokens, i + 1, close_bracket_pos, is_error);
        } else {
            expression.is_left_expr = true;
            expression.left_operand = (struct Relational_Expression *) malloc(sizeof(struct Relational_Expression));
            *((struct Relational_Expression *) (expression.left_operand)) = get_relational_expression(
                    tokens, i + 1, close_bracket_pos, is_error);
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


struct Single_Declaration get_single_declaration(void** tokens, int start, int end, int* is_error) {
    struct Single_Declaration declaration;
    struct Token* token = ((struct Token*)(tokens[0]));

    if (token->type == INT) {
        declaration.type = "int";
    } else if (token->type == CHAR) {
        declaration.type = "char";
    } else {
        declaration.type = "double";
    }

    declaration.var_name = ((struct Token*)(tokens[1]))->attributes->text;
    return declaration;
}


struct Complex_Declaration get_complex_declaration(void** tokens, int start, int end, int* is_error) {
    struct Complex_Declaration complex_declaration;
    struct Token* token = ((struct Token*)(tokens[start]));

    if (token->type == INT) {
        complex_declaration.type = "int";
    } else if (token->type == CHAR) {
        complex_declaration.type = "char";
    } else {
        complex_declaration.type = "double";
    }

    int counter = 1;
    for (int i = start + 1; i < end; ++i) {
        if (((struct Token*)tokens[i])->type == IDENTIFIER) {
            ++counter;
        }
    }

    complex_declaration.var_names = (char**)malloc(counter * sizeof(char));
    complex_declaration.quantity_of_variables = counter;
    int j = start + 1;
    for (int i = 0; i < counter; ++i) {
        complex_declaration.var_names[i] = ((struct Token*)(tokens[j]))->attributes->text;
        j += 2;
    }

    return complex_declaration;
}


struct Single_Definition get_single_definition(void** tokens, int start, int end, int* is_error) {
    int position = start;
    struct Single_Definition definition;
    struct Token* token = ((struct Token*)(tokens[position]));

    if (token->type == INT) {
        definition.type = "int";
    } else if (token->type == CHAR) {
        definition.type = "char";
    } else {
        definition.type = "double";
    }

    ++position;
    definition.var_name = ((struct Token*)(tokens[position]))->attributes->text;
    definition.is_expression = false;
    definition.is_variable = false;
    position += 2;
    if (position == end) {
        if (((struct Token*)(tokens[position]))->type == IDENTIFIER) {
            definition.is_variable = true;
        }
        definition.expression = ((struct Token*)(tokens[position]))->attributes->text;
    } else {
        if (is_arithmetic_expression(tokens, &position, end, is_error)) {
            definition.expression = malloc(sizeof(struct Arithmetic_Expression));
            struct Arithmetic_Expression* expr =
                    (struct Arithmetic_Expression*)malloc(sizeof(struct Arithmetic_Expression));
            *expr = get_arithmetic_expression(tokens, position, end, is_error);
            definition.expression = (void*)expr;
            definition.is_expression = true;
        }
    }

    return definition;
}


struct Complex_Definition get_complex_definition(void** tokens, int start, int end, int* is_error) {

}


struct If_Else get_if_else_statement(void** tokens, int start, int end, int* is_error) {
    int i = start + 1; // Start of logic expression
    struct If_Else if_else_statement;
    int conditions_quantity = 1;
    int bodies_quantity = 1;

    int round_close_index;
    int curly_close_index;

_check_if_else:
    round_close_index = find_close_bracket_2(tokens, i, end);
    i = round_close_index + 1;
    curly_close_index = find_close_curly_bracket(tokens, i, end);
    i = curly_close_index + 1;
    if (((struct Token*)(tokens[i]))->type == ELSE) {
        if (((struct Token*)(tokens[i + 1]))->type == IF) {
            ++conditions_quantity;
            ++bodies_quantity;
            i += 2;
            goto _check_if_else;
        }
        ++bodies_quantity;
    }

    if_else_statement.quantity_of_conditions = conditions_quantity;
    if_else_statement.quantity_of_bodies = bodies_quantity;
    if_else_statement.conditions = malloc(conditions_quantity * sizeof(void*));
    if_else_statement.bodies = malloc(bodies_quantity * sizeof(void*));

    int* grammars_quantities = (int*)malloc(bodies_quantity * sizeof(int));
    for (int j = 0; j < bodies_quantity; ++j) {
        *grammars_quantities = 0;
    }

    i = start + 1;
    for (int j = 0; j < conditions_quantity; ++j) {
        round_close_index = find_close_bracket_2(tokens, i, end);
        ++i;
        if (is_logical_expression(tokens, &i, round_close_index - 1, is_error)) {
            struct Grammar* grammar = (struct Grammar*)malloc(sizeof(struct Grammar));
            struct Logic_Expression* expression = (struct Logic_Expression*)malloc(sizeof(struct Logic_Expression));
            *expression = get_logic_expression(tokens, i, round_close_index - 1, is_error);
            grammar->data = expression;
            if_else_statement.conditions[j] = *grammar;
        } else {
            struct Grammar* grammar = (struct Grammar*)malloc(128 * sizeof(struct Grammar));
            struct Relational_Expression* expression
                    = (struct Relational_Expression*)malloc(sizeof(struct Relational_Expression));
            *expression = get_relational_expression(tokens, i, round_close_index - 1, is_error);
            grammar->data = expression;
            if_else_statement.conditions[j] = *grammar;
        }
        i = round_close_index + 1;
        curly_close_index = find_close_curly_bracket(tokens, i, end);
        int quantity_of_grammars = 0;
        struct Grammar* grammar = get_intermediate_representation_for_scope(
                tokens, i + 1, curly_close_index - 1, &quantity_of_grammars, is_error);
        grammars_quantities[j] = quantity_of_grammars;
        if_else_statement.quantities_of_variables = (int*)malloc(16 * sizeof(int));
        if_else_statement.quantities_of_variables[j] = get_quantity_of_variables(grammar, quantity_of_grammars);
        if_else_statement.bodies[j] = grammar;
        i = curly_close_index + 3;
    }

    if_else_statement.grammars_quantities = grammars_quantities;
    return if_else_statement;
}


struct While get_while_statement(void** tokens, int start, int end, int* is_error) {
    int i = start + 1;
    struct While while_statement;
    int closed_round_index = find_close_bracket_2(tokens, i, end);
    ++i;
    if (is_logical_expression(tokens, &i, closed_round_index - 1, is_error)) {
        struct Grammar* grammar = (struct Grammar*)malloc(sizeof(struct Grammar));
        struct Logic_Expression* expression = (struct Logic_Expression*)malloc(sizeof(struct Logic_Expression));
        *expression = get_logic_expression(tokens, i, closed_round_index - 1, is_error);
        grammar->data = expression;
        while_statement.condition = *grammar;
    } else {
        struct Grammar* grammar = (struct Grammar*)malloc(128 * sizeof(struct Grammar));
        struct Relational_Expression* expression
                = (struct Relational_Expression*)malloc(sizeof(struct Relational_Expression));
        *expression = get_relational_expression(tokens, i, closed_round_index - 1, is_error);
        grammar->data = expression;
        while_statement.condition = *grammar;
    }

    i = closed_round_index + 1;
    int* quantity_of_grammars = (int*)malloc(1 * sizeof(int));
    *quantity_of_grammars = 0;
    struct Grammar* grammars;
    int closed_curly_index = find_close_curly_bracket(tokens, i, end);
    grammars = get_intermediate_representation_for_scope(tokens, i + 1, closed_curly_index - 1, quantity_of_grammars, is_error);
    while_statement.body = grammars;
    while_statement.grammars_quantity = *quantity_of_grammars;
    while_statement.quantity_of_variables = get_quantity_of_variables(grammars, while_statement.grammars_quantity);
    i = closed_curly_index + 2;


    return while_statement;
}


struct Do_While get_do_while_statement(void** tokens, int start, int end, int* is_error) {
    int i = start + 1;
    struct Do_While do_while_statement;
    int closed_curly_index = find_close_curly_bracket(tokens, i, end);
    int* quantity_of_grammars = (int*)malloc(1 * sizeof(int));
    *quantity_of_grammars = 0;
    struct Grammar* grammars;
    grammars = get_intermediate_representation_for_scope(tokens, i + 1, closed_curly_index, quantity_of_grammars, is_error);
    do_while_statement.body = grammars;
    do_while_statement.grammars_quantity = *quantity_of_grammars;
    do_while_statement.quantity_of_variables = get_quantity_of_variables(grammars, do_while_statement.grammars_quantity);
    i = closed_curly_index + 2;
    int closed_round_index = find_close_bracket_2(tokens, i, end);
    ++i;
    if (is_logical_expression(tokens, &i, closed_round_index - 1, is_error)) {
        struct Grammar* grammar = (struct Grammar*)malloc(sizeof(struct Grammar));
        struct Logic_Expression* expression = (struct Logic_Expression*)malloc(sizeof(struct Logic_Expression));
        *expression = get_logic_expression(tokens, i, closed_round_index - 1, is_error);
        grammar->data = expression;
        do_while_statement.condition = *grammar;
    } else {
        struct Grammar* grammar = (struct Grammar*)malloc(128 * sizeof(struct Grammar));
        struct Relational_Expression* expression
                = (struct Relational_Expression*)malloc(sizeof(struct Relational_Expression));
        *expression = get_relational_expression(tokens, i, closed_round_index - 1, is_error);
        grammar->data = expression;
        do_while_statement.condition = *grammar;
    }

    return do_while_statement;
}


struct For get_for_statement(void** tokens, int start, int end, int* is_error) {

}


bool check_string_is_number(char* string) {
    for (int i = 0; i < strlen((string)); ++i) {
        if (string[i] < '0' || string[i] > '9') {
            return false;
        }
    }

    return true;
}


struct Assignment_Expression get_assignment_expression(void** tokens, int start, int end, int* is_error) {
    struct Assignment_Expression expression;
    int i = start;
    expression.var_name = ((struct Token*)(tokens[i]))->attributes->text;
    expression.sign = ((struct Token*)(tokens[i + 1]))->attributes->text;
    expression.is_value_expression = false;
    expression.is_variable = false;

    if (end - i - 2 > 1) {
        struct Arithmetic_Expression* arithm = (struct Arithmetic_Expression*)malloc(sizeof(struct Arithmetic_Expression));
        *arithm = get_arithmetic_expression(tokens, i + 2, end, is_error);
        expression.value = arithm;
        expression.is_value_expression = true;
    } else {
        expression.value = ((struct Token*)(tokens[i + 2]))->attributes->text;
        if (!check_string_is_number(expression.value)) {
            expression.is_variable = true;
        }
    }

    return expression;
}


struct General_Assignment_Expression get_general_assignment_expression(
        void** tokens, int start, int end, int* is_error) {
    struct General_Assignment_Expression expression;
    int i = start;
    expression.var_name = ((struct Token*)(tokens[i]))->attributes->text;
    expression.is_value_expression = false;
    expression.is_variable = false;

    if (end - i - 2 > 1) {
        struct Arithmetic_Expression* arithm =
                (struct Arithmetic_Expression*)malloc(sizeof(struct Arithmetic_Expression));
        *arithm = get_arithmetic_expression(tokens, i + 2, end, is_error);
        expression.value = arithm;
        expression.is_value_expression = true;
    } else {
        expression.value = ((struct Token*)(tokens[i + 2]))->attributes->text;
        if (!check_string_is_number(expression.value)) {
            expression.is_variable = true;
        }
    }

    return expression;
}


struct Read_Int_Function get_read_int(void** tokens, int start, int end, int* is_error) {
    int i = start + 2;
    struct Read_Int_Function read_int;

    read_int.var_name = ((struct Token*)(tokens[i]))->attributes->text;
    return read_int;
}


struct Write_Int_Function get_write_int(void** tokens, int start, int end, int* is_error) {
    int i = start + 2;
    struct Write_Int_Function write_int;

    if (((struct Token*)(tokens[i]))->type == IDENTIFIER) {
        write_int.is_var = true;
    } else {
        write_int.is_var = false;
    }

    write_int.value = ((struct Token*)(tokens[i]))->attributes->text;
    return write_int;
}