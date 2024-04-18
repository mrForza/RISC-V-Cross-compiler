#include "../parser/complex_structures.c"


struct Variable {
    char* alias;
    char* value;
};


struct Stack_Context {
    struct Variable* variables;
    int size;
    int capacity;
};


int get_variable_context_shift(struct Stack_Context context, char* var_name);


struct Stack_Context* init_stack_context(int capacity);


void add_variable_in_stack_context(struct Stack_Context* context, struct Variable* variable);


char* generate_assembly_for_var_declaration(struct Single_Declaration single_declaration);


char* generate_assembly_for_variables_declarations(struct Complex_Declaration complex_declaration);


char* generate_assembly_for_var_definition(struct Single_Definition definition_expression);


char* generate_assembly_for_variables_definitions(char* type, char** var_names, char** values, unsigned long long count);


char* create_variable_alias(char* varname, int shift);


char* generate_assembly_for_arithmetic_expression(struct Arithmetic_Expression expression);


char* generate_assembly_for_logic_expression(struct Logic_Expression expression);


char* generate_assembly_for_relational_expression(struct Relational_Expression expression, char* label,
        char* cancel_label, bool key);


char* generate_assembly_for_if_else_statement(struct If_Else statement);


char* generate_assembly_for_do_while_statement(struct Do_While statement);


char* generate_assembly_for_while_statement(struct While statement);


char* generate_assembly_for_for_statement(struct For statement);


char* generate_assembly_for_assignment_expression(struct Assignment_Expression expression);


char* generate_assembly_for_general_assignment_expression(struct General_Assignment_Expression expression);


char* generate_assembly_for_read_int_function(struct Read_Int_Function read_int);


char* generate_assembly_for_write_int_function(struct Write_Int_Function write_int);
