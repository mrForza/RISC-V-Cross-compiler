#include "code_generator.h"
#include "string.h"
#include "stdio.h"


int stack_pointer = 0x7fffeffc;

int base_stack_pointer = 0x7fffeffc;

int stack_fullness = 0;


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


char* generate_assembly_for_var_definition(char* type, char* var_name, char* value) {
    char* risc_v_assembly = create_variable_alias(var_name, base_stack_pointer - stack_pointer);
    risc_v_assembly = concatenate(risc_v_assembly, ":\n\t");

    if (strcmp(type, "float") == 0) {

    } else {
        risc_v_assembly = concatenate(
                risc_v_assembly,
                "addi sp, sp, -4\n\tli t0 "
        );
        risc_v_assembly = concatenate(risc_v_assembly, value);

        if (strcmp(type, "char") == 0 || strcmp(type, "short") == 0) {
            risc_v_assembly = concatenate(risc_v_assembly,"\n\tsb t0 (sp)\n\0");
        } else {
            risc_v_assembly = concatenate(risc_v_assembly, "\n\tsw t0 (sp)\n\0");
        }
    }

    stack_pointer -= 4;
    return risc_v_assembly;
}


char* generate_assembly_for_variables_definitions(char* type, char** var_names, char** values, unsigned long long count) {
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
        risc_v_assembly = concatenate(risc_v_assembly, "sw t0, (sp)\n\taddi sp, sp, -4\n\t");
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