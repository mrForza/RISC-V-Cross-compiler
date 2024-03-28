#include "code_generator.h"
#include "string.h"
#include "../utils.c"


int stack_pointer = 0x7fffeffc;

int base_stack_pointer = 0x7fffeffc;


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
