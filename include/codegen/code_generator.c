#include "string.h"
#include "stdio.h"
#include "../utils.c"
#include "code_generator.h"


struct Stack_Context* stack_context;

int stack_pointer = 0x7fffeffc;

int base_stack_pointer = 0x7fffeffc;

bool is_initial_generation = true;

int assignment_counter = 0;


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
    struct Single_Declaration single_declaration;
    struct Complex_Declaration complex_declaration;
    struct Single_Definition single_definition;
    struct Arithmetic_Expression arithmetic_expression;
    struct Relational_Expression relational_expression;
    struct If_Else if_else_statement;
    struct Do_While do_while_statement;
    struct While while_statement;
    struct Assignment_Expression assignment_expression;
    struct General_Assignment_Expression general_assignment_expression;
    struct Read_Int_Function read_int;
    struct Write_Int_Function write_int;
    char* risc_v_assembly = "";

    switch (grammar.type) {
        case SINGLE_DECLARATION:
            single_declaration = *(struct Single_Declaration*)grammar.data;
            risc_v_assembly = concatenate(risc_v_assembly, generate_assembly_for_var_declaration(single_declaration));
            break;
        case COMPLEX_DECLARATION:
            complex_declaration = *(struct Complex_Declaration*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_variables_declarations(complex_declaration));
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
            relational_expression = *(struct Relational_Expression*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_relational_expression(relational_expression, "", "", true));
            break;
        case IF_ELSE_STATEMENT:
            if_else_statement = *(struct If_Else*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_if_else_statement(if_else_statement));
            break;
        case DO_WHILE_STATEMENT:
            do_while_statement = *(struct Do_While*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_do_while_statement(do_while_statement));
            break;
        case WHILE_STATEMENT:
            while_statement = *(struct While*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_while_statement(while_statement));
            break;
        case FOR_STATEMENT:
            break;
        case FUNCTION_DECLARATION:
            break;
        case ASSIGNMENT_EXPRESSION:
            assignment_expression = *(struct Assignment_Expression*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_assignment_expression(assignment_expression));
            break;
        case GENERAL_ASSIGNMENT_EXPRESSION:
            general_assignment_expression = *(struct General_Assignment_Expression*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_general_assignment_expression(
                            general_assignment_expression));
            break;
        case READ_INT:
            read_int = *(struct Read_Int_Function*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_read_int_function(read_int));
            break;
        case WRITE_INT:
            write_int = *(struct Write_Int_Function*)grammar.data;
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    generate_assembly_for_write_int_function(write_int));
            break;
    }

    return risc_v_assembly;
}


char* generate_assembly_for_grammars(struct Grammar* grammars, int size) {
    char* risc_v_assembly = "";
    if (is_initial_generation) {
        stack_context = init_stack_context(128);
        is_initial_generation = false;
    }

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


char* generate_assembly_for_var_declaration(struct Single_Declaration single_declaration) {
    char* type = single_declaration.type;
    char* var_name = single_declaration.var_name;
    char* risc_v_assembly = create_variable_alias(var_name, base_stack_pointer - stack_pointer);
    risc_v_assembly = concatenate(risc_v_assembly, ":\n\t");
    if (strcmp(type, "float") == 0) {
        risc_v_assembly = concatenate(
                risc_v_assembly,
                "\n\taddi sp, sp, -4\n\tli ft0 0\n\tflw ft0 (t0)\n\tsw ft0 (sp)\n\0"
        );
    } else {
        risc_v_assembly = concatenate(
                risc_v_assembly,
                "\n\tli t0 0\n\tsw t0 (sp)\n\taddi sp, sp, -4\n\0"
        );
    }

    struct Variable* variable = (struct Variable*)malloc(sizeof(struct Variable));
    variable->alias = var_name;
    variable->value = "0";
    add_variable_in_stack_context(stack_context, variable);

    stack_pointer -= 4;
    return risc_v_assembly;
}


char* generate_assembly_for_variables_declarations(struct Complex_Declaration complex_declaration) {
    char* type = complex_declaration.type;
    char** var_names = complex_declaration.var_names;
    int count = complex_declaration.quantity_of_variables;;
    char* risc_v_assembly = "";
    int index = 0;
    while (count-- > 0) {
        struct Single_Declaration single_declaration;
        single_declaration.type = type;
        single_declaration.var_name = var_names[index++];
        risc_v_assembly = concatenate(
    risc_v_assembly,
    generate_assembly_for_var_declaration(single_declaration)
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
        if (check_string_is_number(expression.left_operand)) {
            risc_v_assembly = concatenate(risc_v_assembly,
                                          concatenate("\n\tli t0, ", expression.left_operand));
        } else {
            int index = get_variable_context_shift(*stack_context, (char*)expression.left_operand);
            if (index  == -1) {
                printf("SEMANTIC ERROR: No variable %s in current stack scope!", (char*)expression.left_operand);
                return "";
            }
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\taddi, sp, sp, ",
                                convert_int_to_string((stack_context->size - index) * 4)));
            risc_v_assembly = concatenate(risc_v_assembly, "\n\tlw, t0, (sp)");
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate(
                            "\n\taddi, sp, sp, -",
                            convert_int_to_string((stack_context->size - index) * 4)));
        }
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
        if (check_string_is_number(expression.right_operand)) {
            risc_v_assembly = concatenate(risc_v_assembly, "\n\tli t1, ");
            risc_v_assembly = concatenate(risc_v_assembly, (char*)expression.right_operand);
        } else {
            int index = get_variable_context_shift(*stack_context, (char*)expression.right_operand);
            if (index  == -1) {
                printf("SEMANTIC ERROR: No variable %s in current stack scope!", (char*)expression.right_operand);
                return "";
            }
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\taddi, sp, sp, ",
                                convert_int_to_string((stack_context->size - index) * 4)));
            risc_v_assembly = concatenate(risc_v_assembly, "\n\tlw, t1, (sp)");
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate(
                            "\n\taddi, sp, sp, -",
                            convert_int_to_string((stack_context->size - index) * 4)));
        }
        goto _operators;
    }

    _operators:
    if (strcmp(expression.operator, "+") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tadd t0, t0, t1");
    } else if (strcmp(expression.operator, "-") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tsub t0, t0, t1");
    } else if (strcmp(expression.operator, "*") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tmul t0, t0, t1");
    } else if (strcmp(expression.operator, "/") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tdiv t0, t0, t1");
    } else if (strcmp(expression.operator, "%") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\trem t0, t0, t1");
    } else if (strcmp(expression.operator, "&") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tand t0, t0, t1");
    } else if (strcmp(expression.operator, "|") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tor t0, t0, t1");
    } else if (strcmp(expression.operator, "^") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\txor t0, t0, t1");
    }

    risc_v_assembly = concatenate(risc_v_assembly,
                                  "\n\tsw t0, (sp)\n\taddi, sp, sp, -4\n\0");

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
                risc_v_assembly = concatenate(
                        risc_v_assembly,
                        concatenate("\n\taddi, sp, sp, ",
                                    convert_int_to_string((stack_context->size - index) * 4)));
                risc_v_assembly = concatenate(risc_v_assembly, "\n\tlw, t0, (sp)");
                risc_v_assembly = concatenate(
                        risc_v_assembly,
                        concatenate(
                                "\n\taddi, sp, sp, -",
                                convert_int_to_string((stack_context->size - index) * 4)));
                risc_v_assembly = concatenate(risc_v_assembly, "\n\tsw, t0, (sp)");
            } else {
                risc_v_assembly = concatenate(risc_v_assembly, "\n\tli t0, ");
                risc_v_assembly = concatenate(risc_v_assembly, value);

                if (strcmp(type, "char") == 0 || strcmp(type, "short") == 0) {
                    risc_v_assembly = concatenate(risc_v_assembly,"\n\tsb t0 (sp)\n\0");
                } else {
                    risc_v_assembly = concatenate(risc_v_assembly, "\n\tsw t0 (sp)");
                }
            }
            risc_v_assembly = concatenate(risc_v_assembly, "\n\taddi, sp, sp, -4\n\0");
        }

        struct Variable* var = (struct Variable*)malloc(sizeof(struct Variable));
        var->alias = var_name;
        var->value = value;
        add_variable_in_stack_context(stack_context, var);
    }

    stack_pointer -= 4;
    return risc_v_assembly;
}


/*char* generate_assembly_for_variables_definitions(char* type, char** var_names, char** values, int count) {
    char* risc_v_assembly = "";
    int index = 0;
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


char* generate_assembly_for_relational_expression(struct Relational_Expression expression, char* label,
        char* cancel_label, bool key) {
    char* risc_v_assembly = "RELATION:";

    if (expression.is_left_expr) {
        risc_v_assembly = concatenate(risc_v_assembly, generate_assembly_for_arithmetic_expression(
                *((struct Arithmetic_Expression*)(expression.left_operand))));
        risc_v_assembly = concatenate(risc_v_assembly,
                                      "\n\taddi, sp, sp, 4\n\tlw t0, (sp)\n\tsw, t6, (sp)");
    } else {
        if (check_string_is_number(expression.left_operand)) {
            risc_v_assembly = concatenate(risc_v_assembly,
                                          concatenate("\n\tli t0, ", expression.left_operand));
        } else {
            int index = get_variable_context_shift(*stack_context, (char*)expression.left_operand);
            if (index  == -1) {
                printf("SEMANTIC ERROR: No variable %s in current stack scope!", (char*)expression.left_operand);
                return "";
            }
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\taddi, sp, sp, ",
                                convert_int_to_string((stack_context->size - index) * 4)));
            risc_v_assembly = concatenate(risc_v_assembly, "\n\tlw, t0, (sp)");
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate(
                            "\n\taddi, sp, sp, -",
                            convert_int_to_string((stack_context->size - index) * 4)));
        }
    }


    if (expression.is_right_expr) {
        risc_v_assembly = concatenate(risc_v_assembly, generate_assembly_for_arithmetic_expression(
                *((struct Arithmetic_Expression*)(expression.right_operand))));
        risc_v_assembly = concatenate(risc_v_assembly,
                                      "addi, sp, sp, 4\n\tlw t1, (sp)\n\tsw, t6, (sp)\n\t");
        risc_v_assembly = concatenate(risc_v_assembly,
                                      "addi, sp, sp, 4\n\tlw t0, (sp)\n\tsw, t6, (sp)\n\t");
    } else {
        if (check_string_is_number(expression.right_operand)) {
            risc_v_assembly = concatenate(risc_v_assembly, "\n\tli t1, ");
            risc_v_assembly = concatenate(risc_v_assembly, (char*)expression.right_operand);
        } else {
            int index = get_variable_context_shift(*stack_context, (char*)expression.right_operand);
            if (index  == -1) {
                printf("SEMANTIC ERROR: No variable %s in current stack scope!", (char*)expression.right_operand);
                return "";
            }
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\taddi, sp, sp, ",
                                convert_int_to_string((stack_context->size - index) * 4)));
            risc_v_assembly = concatenate(risc_v_assembly, "\n\tlw, t1, (sp)");
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate(
                            "\n\taddi, sp, sp, -",
                            convert_int_to_string((stack_context->size - index) * 4)));
        }
        goto _operators;
    }

_operators:
    if (key) {
        if (strcmp(expression.operator, "==") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tbeq t0, t1, ", label));
        } else if (strcmp(expression.operator, "!=") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tbne t0, t1, ", label));
        } else if (strcmp(expression.operator, "<") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tblt t0, t1, ", label));
        } else if (strcmp(expression.operator, ">") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tbgt t0, t1, ", label));
        } else if (strcmp(expression.operator, "<=") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tble t0, t1, ", label));
        } else if (strcmp(expression.operator, ">=") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tbge t0, t1, ", label));
        }
    } else {
        if (strcmp(expression.operator, "==") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tbne t0, t1, ", cancel_label));
        } else if (strcmp(expression.operator, "!=") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tbeq t0, t1, ", cancel_label));
        } else if (strcmp(expression.operator, "<") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tbge t0, t1, ", cancel_label));
        } else if (strcmp(expression.operator, ">") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tble t0, t1, ", cancel_label));
        } else if (strcmp(expression.operator, "<=") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tbgt t0, t1, ", cancel_label));
        } else if (strcmp(expression.operator, ">=") == 0) {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tblt t0, t1, ", cancel_label));
        }
    }

    return risc_v_assembly;
}


char* generate_assembly_for_if_else_statement(struct If_Else statement) {
    char* risc_v_assembly = "";
    char* risc_v_assembly_for_bodies = "";

    // ADD GLOBAL LABEL COUNTER !!!!!!!
    for (int i = 0; i < statement.quantity_of_bodies; ++i) {
        risc_v_assembly_for_bodies = concatenate(
                risc_v_assembly_for_bodies,
                concatenate("\n\tbody_", convert_int_to_string(i)));
        risc_v_assembly_for_bodies = concatenate(risc_v_assembly_for_bodies, ":\n\t");
        risc_v_assembly_for_bodies = concatenate(
                risc_v_assembly_for_bodies,
                generate_assembly_for_grammars(
                        statement.bodies[i], statement.grammars_quantities[i]));
        risc_v_assembly_for_bodies = concatenate(risc_v_assembly_for_bodies, "\n\tj end\n\t");
    }

    for (int i = 0; i < statement.quantity_of_conditions; ++i) {
        risc_v_assembly = concatenate(
                risc_v_assembly,
                generate_assembly_for_relational_expression(
                        *(struct Relational_Expression*)statement.conditions[i].data,
                        concatenate("body_", convert_int_to_string(i)), "end", true));
    }

    risc_v_assembly_for_bodies = concatenate(risc_v_assembly_for_bodies, "\n\tend:");
    risc_v_assembly = concatenate(risc_v_assembly, risc_v_assembly_for_bodies);
    return risc_v_assembly;
}


char* generate_assembly_for_do_while_statement(struct Do_While statement) {
    char* risc_v_assembly = "";

_new_iteration:
    risc_v_assembly = concatenate(risc_v_assembly, "do_while_body:\n\t");
    for (int i = 0; i < statement.grammars_quantity; ++i) {
        risc_v_assembly = concatenate(risc_v_assembly,
                                      generate_assembly_for_unknown_grammar(statement.body[i]));
    }

    risc_v_assembly = concatenate(
            risc_v_assembly,
            generate_assembly_for_relational_expression(
                    *(struct Relational_Expression*)statement.condition.data,
                            "do_while_body", "do_while_end", true));

    return risc_v_assembly;
}


char* generate_assembly_for_while_statement(struct While statement) {
    char* risc_v_assembly = "";

    risc_v_assembly = concatenate(risc_v_assembly, "\n\twhile_condition:\n\t");
    risc_v_assembly = concatenate(
            risc_v_assembly,
            generate_assembly_for_relational_expression(
                    *(struct Relational_Expression*)statement.condition.data,
                    "while_body", "while_end",  false));

_new_iteration:
    risc_v_assembly = concatenate(risc_v_assembly, "\n\twhile_body:\n\t");
    for (int i = 0; i < statement.grammars_quantity; ++i) {
        risc_v_assembly = concatenate(risc_v_assembly,
                                      generate_assembly_for_unknown_grammar(statement.body[i]));
    }
    risc_v_assembly = concatenate(risc_v_assembly, "\n\tj while_condition");
    risc_v_assembly = concatenate(risc_v_assembly, "\n\twhile_end:");
    return risc_v_assembly;
}


char* generate_assembly_for_for_statement(struct For statement) {
    char* risc_v_assembly = "";

    return risc_v_assembly;
}


char* generate_assembly_for_assignment_expression(struct Assignment_Expression expression) {
    char* representation = malloc(2 * sizeof(char));
    sprintf(representation, "%d", assignment_counter++);
    char* risc_v_assembly = concatenate("ASSIGNMENT_", representation);
    risc_v_assembly = concatenate(risc_v_assembly, ":\n\t");

    int index = get_variable_context_shift(*stack_context, (char*)expression.var_name);
    if (index  == -1) {
        printf("SEMANTIC ERROR: No variable %s in current stack scope!", (char*)expression.var_name);
        return "";
    }
    risc_v_assembly = concatenate(
            risc_v_assembly,
            concatenate("\n\taddi, sp, sp, ",
                        convert_int_to_string((stack_context->size - index) * 4)));
    risc_v_assembly = concatenate(risc_v_assembly, "\n\tlw, t0, (sp)");
    risc_v_assembly = concatenate(
            risc_v_assembly,
            concatenate(
                    "\n\taddi, sp, sp, -",
                    convert_int_to_string((stack_context->size - index) * 4)));

    if (expression.is_value_expression) {
        risc_v_assembly = concatenate(risc_v_assembly, generate_assembly_for_arithmetic_expression(
                *((struct Arithmetic_Expression*)(expression.value))));
    } else {
        if (expression.is_variable) {
            index = get_variable_context_shift(*stack_context, (char*)expression.value);
            if (index  == -1) {
                printf("SEMANTIC ERROR: No variable %s in current stack scope!", (char*)expression.value);
                return "";
            }
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\taddi, sp, sp, ",
                                convert_int_to_string((stack_context->size - index) * 4)));
            risc_v_assembly = concatenate(risc_v_assembly, "\n\tlw, t1, (sp)");
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate(
                            "\n\taddi, sp, sp, -",
                            convert_int_to_string((stack_context->size - index) * 4)));
        } else {
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\tli t1, ", (char*)expression.value));
        }

    }

    int value = strtol(expression.value, NULL, 10);
    int left = strtol(stack_context->variables[index].value, NULL, 10);
_operators:

    if (strcmp(expression.sign, "+=") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tadd t0, t0, t1");
        sprintf(stack_context->variables[index].value, "%d", left + value);
    } else if (strcmp(expression.sign, "-=") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tsub t0, t0, t1");
        sprintf(stack_context->variables[index].value, "%d", left - value);
    } else if (strcmp(expression.sign, "*=") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tmul t0, t0, t1");
        sprintf(stack_context->variables[index].value, "%d", left * value);
    } else if (strcmp(expression.sign, "/=") == 0) {
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tdiv t0, t0, t1");
        sprintf(stack_context->variables[index].value, "%d", left / value);
    }

    int current_stack_pointer = base_stack_pointer - (4 * index);
    risc_v_assembly = concatenate(risc_v_assembly,
                                  concatenate("\n\tli sp, ",
                                              convert_int_to_string(current_stack_pointer)));
    risc_v_assembly = concatenate(risc_v_assembly, "\n\tsw, t0, (sp)");
    risc_v_assembly = concatenate(risc_v_assembly,
                                  concatenate("\n\tli sp, ",
                                              convert_int_to_string(stack_pointer)));
    risc_v_assembly = concatenate(risc_v_assembly, "\n\tli t0, 0");
    risc_v_assembly = concatenate(risc_v_assembly, "\n\tli t1, 0\n\0");

    return risc_v_assembly;
}


char* generate_assembly_for_general_assignment_expression(struct General_Assignment_Expression expression) {
    char* risc_v_assembly = "";
    char* var_name = expression.var_name;
    char* value = "0";
    int index;

    if (expression.is_value_expression) {
        risc_v_assembly = concatenate(
                risc_v_assembly,
                generate_assembly_for_arithmetic_expression(
                        *(struct Arithmetic_Expression*)expression.value));
        risc_v_assembly = concatenate(risc_v_assembly,
                                      "\n\taddi, sp, sp, 4\n\tlw t0, (sp)\n\tsw, t6, (sp)");
    } else {
        value = (char*)expression.value;
        if (expression.is_variable) {
            index = get_variable_context_shift(*stack_context, value);
            if (index  == -1) {
                printf("SEMANTIC ERROR: No variable %s in current stack scope!", value);
                return "";
            }
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate("\n\taddi, sp, sp, ",
                                convert_int_to_string((stack_context->size - index) * 4)));
            risc_v_assembly = concatenate(risc_v_assembly, "\n\tlw, t0, (sp)");
            risc_v_assembly = concatenate(
                    risc_v_assembly,
                    concatenate(
                            "\n\taddi, sp, sp, -",
                            convert_int_to_string((stack_context->size - index) * 4)));
        } else {
            risc_v_assembly = concatenate(risc_v_assembly, "\n\tli t0, ");
            risc_v_assembly = concatenate(risc_v_assembly, value);
        }
    }

    index = get_variable_context_shift(*stack_context, var_name);
    if (index  == -1) {
        printf("SEMANTIC ERROR: No variable %s in current stack scope!", var_name);
        return "";
    }
    risc_v_assembly = concatenate(
            risc_v_assembly,
            concatenate("\n\taddi, sp, sp, ",
                        convert_int_to_string((stack_context->size - index) * 4)));
    risc_v_assembly = concatenate(risc_v_assembly, "\n\tsw, t0, (sp)");
    risc_v_assembly = concatenate(
            risc_v_assembly,
            concatenate(
                    "\n\taddi, sp, sp, -",
                    convert_int_to_string((stack_context->size - index) * 4)));
    risc_v_assembly = concatenate(risc_v_assembly, "\n\t");
    return risc_v_assembly;
}


char* generate_assembly_for_read_int_function(struct Read_Int_Function read_int) {
    char* risc_v_assembly = "\n\tli a7, 5";
    risc_v_assembly = concatenate(risc_v_assembly, "\n\tecall");

    int index = get_variable_context_shift(*stack_context, read_int.var_name);
    if (index  == -1) {
        printf("SEMANTIC ERROR: No variable %s in current stack scope!", read_int.var_name);
        return "";
    }
    risc_v_assembly = concatenate(risc_v_assembly, "\n\tmv t0, a0");
    risc_v_assembly = concatenate(
            risc_v_assembly,
            concatenate("\n\taddi, sp, sp, ",
                        convert_int_to_string((stack_context->size - index) * 4)));
    risc_v_assembly = concatenate(risc_v_assembly, "\n\tsw, t0, (sp)");
    risc_v_assembly = concatenate(
            risc_v_assembly,
            concatenate(
                    "\n\taddi, sp, sp, -",
                    convert_int_to_string((stack_context->size - index) * 4)));
    risc_v_assembly = concatenate(risc_v_assembly, "\n");
    return risc_v_assembly;
}


char* generate_assembly_for_write_int_function(struct Write_Int_Function write_int) {
    char* risc_v_assembly = "\n\tli a7, 1";

    if (write_int.is_var) {
        int index = get_variable_context_shift(*stack_context, write_int.value);
        if (index  == -1) {
            printf("SEMANTIC ERROR: No variable %s in current stack scope!", write_int.value);
            return "";
        }
        risc_v_assembly = concatenate(
                risc_v_assembly,
                concatenate("\n\taddi, sp, sp, ",
                            convert_int_to_string((stack_context->size - index) * 4)));
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tlw, t0, (sp)");
        risc_v_assembly = concatenate(
                risc_v_assembly,
                concatenate(
                        "\n\taddi, sp, sp, -",
                        convert_int_to_string((stack_context->size - index) * 4)));
        risc_v_assembly = concatenate(risc_v_assembly, "\n\tmv a0, t0");
    } else {
        risc_v_assembly = concatenate(
                risc_v_assembly,
                concatenate("\n\tli a0, ", write_int.value));
    }

    risc_v_assembly = concatenate(risc_v_assembly, "\n\tecall");
    return risc_v_assembly;
}