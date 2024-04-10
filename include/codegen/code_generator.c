#include "string.h"
#include "stdio.h"
#include "../utils.c"
#include "code_generator.h"


struct Stack_Context* stack_context;

int stack_pointer = 0x7fffeffc;

int base_stack_pointer = 0x7fffeffc;

int stack_fullness = 0;


char* convert_int_to_string(int number) {
    int temp = number;
    int quantity_of_bits = 0;

    while (temp > 0) {
        ++quantity_of_bits;
        temp /= 10;
    }

    char* string = (char*)malloc(quantity_of_bits * sizeof(char));
    sprintf(string, "%d", number);
    return string;
}


struct Stack_Context* init_stack_context(int capacity) {
    struct Stack_Context* context = (struct Stack_Context*)malloc(1 * sizeof(struct Stack_Context));
    context->size = 0;
    context->capacity = capacity;
    context->variables = (struct Variable*)malloc(capacity * sizeof(struct Variable));
    return context;
}


void add_variable_in_stack_context(struct Stack_Context* context, struct Variable* variable) {
    if (context->size >= context->capacity) {
        struct Variable* new_data = realloc(context->variables, 2 * context->capacity);
        context->variables = new_data;
        context->capacity *= 2;
    }
    context->variables[context->size++] = *variable;
}


int get_variable_context_shift(struct Stack_Context context, char* var_name) {
    for (int i = 0; i < context.size; ++i) {
        if (strcmp(context.variables[i].alias, var_name) == 0) {
            return i;
        }
    }
    return -1;
}


char* generate_assembly_for_unknown_grammar(struct Grammar grammar) {
    struct Single_Definition single_definition;
    struct Arithmetic_Expression arithmetic_expression;
    char* risc_v_assembly = "";
    switch (grammar.type) {
        case SINGLE_DECLARATION:
            break;
        case COMPLEX_DECLARATION:
            break;
        case SINGLE_DEFINITION:
            single_definition = *(struct Single_Definition*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_var_definition(single_definition));
            break;
        case COMPLEX_DEFINITION:
            break;
        case ARITHMETIC_EXPRESSION:
            arithmetic_expression = *(struct Arithmetic_Expression*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_arithmetic_expression(arithmetic_expression));
            break;
        case LOGIC_EXPRESSION:
            break;
        case RELATIONAL_EXPRESSION:
            break;
        case IF_ELSE_STATEMENT:
            break;
        case DO_WHILE_STATEMENT:
            break;
        case WHILE_STATEMENT:
            break;
        case FOR_STATEMENT:
            break;
        case FUNCTION_DECLARATION:
            break;
    }

    return risc_v_assembly;
}


char* generate_assembly_for_grammars(struct Grammar* grammars, int size) {
    char* risc_v_assembly = "";
    stack_context = init_stack_context(128);
    for (int i = 0; i < size; ++i) {
        risc_v_assembly = concatenate(risc_v_assembly, generate_assembly_for_unknown_grammar(grammars[i]));
    }
    return risc_v_assembly;
}


char* create_variable_alias(char* varname, int shift) {
    char* variable_alias = concatenate(varname, "_");

    int temp = shift;
    int count_of_bits = 0;
    while (temp > 0) {
        temp /= 10;
        ++count_of_bits;
    }

    if (count_of_bits == 0) {
        count_of_bits = 1;
    }
    char* shift_representation = malloc(count_of_bits * sizeof(char));

    sprintf(shift_representation, "%d", shift);
    variable_alias = concatenate(variable_alias, shift_representation);
    return variable_alias;
}


char* generate_assembly_for_var_declaration(char* type, char* var_name) {
    char* risc_v_assembly = create_variable_alias(var_name, base_stack_pointer - stack_pointer);
    risc_v_assembly = concatenate(risc_v_assembly, ":\n\t");
    if (strcmp(type, "float") == 0) {
        risc_v_assembly = concatenate(
                risc_v_assembly,
                "addi sp, sp, -4\n\tli ft0 0\n\tflw ft0 (t0)\n\tsw ft0 (sp)\n\0"
        );
    } else {
        risc_v_assembly = concatenate(
                risc_v_assembly,
                "addi sp, sp, -4\n\tli t0 0\n\tsw t0 (sp)\n\0"
        );
    }

    stack_pointer -= 4;
    return risc_v_assembly;
}


char* generate_assembly_for_variables_declarations(char* type, char** var_names, unsigned long long count) {
    char* risc_v_assembly = "";
    unsigned long long index = 0;
    while (count-- > 0) {
        risc_v_assembly = concatenate(
    risc_v_assembly,
    generate_assembly_for_var_declaration(type, var_names[index++])
        );
    }
    return risc_v_assembly;
}


char* generate_assembly_for_arithmetic_expression(struct Arithmetic_Expression expression) {
    char* risc_v_assembly = "";

    if (expression.is_left_expr) {
        risc_v_assembly = concatenate(risc_v_assembly, generate_assembly_for_arithmetic_expression(
                *((struct Arithmetic_Expression*)(expression.left_operand))));
    } else {
        risc_v_assembly = concatenate(risc_v_assembly,
                                      concatenate("li t0, ", expression.left_operand));
        risc_v_assembly = concatenate(risc_v_assembly, "\n\t");
        risc_v_assembly = concatenate(
                risc_v_assembly,
                concatenate("\n\tli sp, ", convert_int_to_string(stack_pointer)));
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tsw t0, (sp)\n\taddi sp, sp, -4\n\t");
    }

    if (expression.is_right_expr) {
        risc_v_assembly = concatenate(risc_v_assembly, generate_assembly_for_arithmetic_expression(
                *((struct Arithmetic_Expression*)(expression.right_operand))));
        risc_v_assembly = concatenate(risc_v_assembly,
                                      "addi, sp, sp, 4\n\tlw t1, (sp)\n\tsw, t6, (sp)\n\t");
        risc_v_assembly = concatenate(risc_v_assembly,
                                      "addi, sp, sp, 4\n\tlw t0, (sp)\n\tsw, t6, (sp)\n\t");
    } else {
        risc_v_assembly = concatenate(risc_v_assembly, "addi, sp, sp, 4\n\tlw t0, (sp)\n\t");
        risc_v_assembly = concatenate(risc_v_assembly, "li t1, ");
        risc_v_assembly = concatenate(risc_v_assembly, (char*)expression.right_operand);
        risc_v_assembly = concatenate(risc_v_assembly, "\n\t");
        goto _operators;
    }

    _operators:
    if (strcmp(expression.operator, "+") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "add t0, t0, t1\n\t");
    } else if (strcmp(expression.operator, "-") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "sub t0, t0, t1\n\t");
    } else if (strcmp(expression.operator, "*") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "mul t0, t0, t1\n\t");
    } else if (strcmp(expression.operator, "/") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "div t0, t0, t1\n\t");
    } else if (strcmp(expression.operator, "%") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "rem t0, t0, t1\n\t");
    } else if (strcmp(expression.operator, "&") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "and t0, t0, t1\n\t");
    } else if (strcmp(expression.operator, "|") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "or t0, t0, t1\n\t");
    } else if (strcmp(expression.operator, "^") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "xor t0, t0, t1\n\t");
    }

    risc_v_assembly = concatenate(risc_v_assembly,
                                  "sw t0, (sp)\n\taddi, sp, sp, -4\n\tli t0, 0\n\tli t1, 0\n\t");

    return risc_v_assembly;
}


char* generate_assembly_for_var_definition(struct Single_Definition definition_expression) {
    char* type = definition_expression.type;
    char* var_name = definition_expression.var_name;
    char* value;

    char* risc_v_assembly = create_variable_alias(var_name, base_stack_pointer - stack_pointer);
    risc_v_assembly = concatenate(risc_v_assembly, ":\n\t");

    if (strcmp(type, "float") == 0) {
        // Assembly for float
    } else {
        if (definition_expression.is_expression) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_arithmetic_expression(
                            *(struct Arithmetic_Expression*)definition_expression.expression));
        } else {
            value = (char*)definition_expression.expression;
            if (definition_expression.is_variable) {
                int index = get_variable_context_shift(*stack_context, value);
                if (index  == -1) {
                    printf("SEMANTIC ERROR: No variable %s in current stack scope!", value);
                    return "";
                }
                int current_stack_pointer = base_stack_pointer - (4 * index);
                risc_v_assembly = concatenate(risc_v_assembly,
                                              concatenate("\n\tli sp, ", convert_int_to_string(current_stack_pointer)));
                risc_v_assembly = concatenate(risc_v_assembly, "\n\tlw, t0, (sp)");
                risc_v_assembly = concatenate(risc_v_assembly,
                                              concatenate("\n\tli sp, ", convert_int_to_string(stack_pointer)));
                risc_v_assembly = concatenate(risc_v_assembly, "\n\tsw, t0, (sp)\n");

            } else {
                risc_v_assembly = concatenate(risc_v_assembly, concatenate("\n\tli sp, ", convert_int_to_string(stack_pointer)));
                risc_v_assembly = concatenate(risc_v_assembly, "\n\tli t0, ");
                risc_v_assembly = concatenate(risc_v_assembly, value);

                if (strcmp(type, "char") == 0 || strcmp(type, "short") == 0) {
                    risc_v_assembly = concatenate(risc_v_assembly,"\n\tsb t0 (sp)\n\0");
                } else {
                    risc_v_assembly = concatenate(risc_v_assembly, "\n\tsw t0 (sp)\n\0");
                }
            }

        }

        struct Variable* var = (struct Variable*)malloc(sizeof(struct Variable));
        var->alias = var_name;
        var->value = value;
        add_variable_in_stack_context(stack_context, var);
    }

    stack_pointer -= 4;
    return risc_v_assembly;
}


/*char* generate_assembly_for_variables_definitions(char* type, char** var_names, char** values, unsigned long long count) {
    char* risc_v_assembly = "";
    unsigned long long index = 0;
    while (count-- > 0) {
        risc_v_assembly = concatenate(
                risc_v_assembly,
                generate_assembly_for_var_definition(type, var_names[index], values[index])
        );
        ++index;
    }
    return risc_v_assembly;
}*/


char* generate_assembly_for_logic_expression(struct Logic_Expression expression) {
    char* risc_v_assembly = "";

    if (expression.is_left_expr) {
        risc_v_assembly = concatenate(risc_v_assembly, generate_assembly_for_logic_expression(
                *((struct Logic_Expression*)(expression.left_operand))));
    } else {
        risc_v_assembly = concatenate(risc_v_assembly,
                                      concatenate("li t0, ", expression.left_operand));
        risc_v_assembly = concatenate(risc_v_assembly, "\n\t");
        risc_v_assembly = concatenate(risc_v_assembly, "sw t0, (sp)\n\taddi sp, sp, -4\n\t");
    }

    if (expression.is_right_expr) {
        risc_v_assembly = concatenate(risc_v_assembly, generate_assembly_for_logic_expression(
                *((struct Logic_Expression*)(expression.right_operand))));
        risc_v_assembly = concatenate(risc_v_assembly,
                                      "addi, sp, sp, 4\n\tlw t1, (sp)\n\tsw, t6, (sp)\n\t");
        risc_v_assembly = concatenate(risc_v_assembly,
                                      "addi, sp, sp, 4\n\tlw t0, (sp)\n\tsw, t6, (sp)\n\t");
    } else {
        risc_v_assembly = concatenate(risc_v_assembly, "addi, sp, sp, 4\n\tlw t0, (sp)\n\t");
        risc_v_assembly = concatenate(risc_v_assembly, "li t1, ");
        risc_v_assembly = concatenate(risc_v_assembly, (char*)expression.right_operand);
        risc_v_assembly = concatenate(risc_v_assembly, "\n\t");
        goto _operators;
    }

    _operators:
    if (strcmp(expression.operator, "&&") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "and t0, t0, t1\n\t");
    } else if (strcmp(expression.operator, "||") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "or t0, t0, t1\n\t");
    }

    risc_v_assembly = concatenate(risc_v_assembly,
                                  "sw t0, (sp)\n\taddi, sp, sp, -4\n\tli t0, 0\n\tli t1, 0\n\t");

    return risc_v_assembly;
}


char* generate_assembly_for_relational_expression(struct Relational_Expression expression) {
    char* risc_v_assembly = "";

    return risc_v_assembly;
}


char* generate_assembly_for_if_else_statement(struct If_Else statement) {
    char* risc_v_assembly = "";

    return risc_v_assembly;
}


char* generate_assembly_for_do_while_statement(struct Do_While statement) {
    char* risc_v_assembly = "";

    risc_v_assembly = concatenate(
            risc_v_assembly,
            generate_assembly_for_logic_expression(
                    *((struct Logic_Expression*)(statement.condition.data))));

_new_iteration:
    for (int i = 0; i < statement.grammars_quantity; ++i) {
        risc_v_assembly = concatenate(risc_v_assembly,
                                      generate_assembly_for_unknown_grammar(statement.body[i]));
    }

    // CHECK CONDITION
    // goto _new_iteration;

    return risc_v_assembly;
}


char* generate_assembly_for_while_statement(struct While statement) {
    char* risc_v_assembly = "";

    risc_v_assembly = concatenate(
            risc_v_assembly,
            generate_assembly_for_logic_expression(
                    *((struct Logic_Expression*)(statement.condition.data))));

_new_iteration:
    // CHECK CONDITION

    for (int i = 0; i < statement.grammars_quantity; ++i) {
        risc_v_assembly = concatenate(risc_v_assembly,
                                      generate_assembly_for_unknown_grammar(statement.body[i]));
    }
    // goto _new_iteration;

    return risc_v_assembly;
}


char* generate_assembly_for_for_statement(struct For statement) {
    char* risc_v_assembly = "";

    return risc_v_assembly;
}