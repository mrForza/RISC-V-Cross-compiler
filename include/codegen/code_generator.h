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


char* generate_assembly_for_var_declaration(char* type, char* var_name);


char* generate_assembly_for_variables_declarations(char* type, char** var_names, unsigned long long count);


char* generate_assembly_for_var_definition(struct Single_Definition definition_expression);


char* generate_assembly_for_variables_definitions(char* type, char** var_names, char** values, unsigned long long count);


char* create_variable_alias(char* varname, int shift);


char* generate_assembly_for_arithmetic_expression(struct Arithmetic_Expression expression);


char* generate_assembly_for_logic_expression(struct Logic_Expression expression);


char* generate_assembly_for_relational_expression(struct Relational_Expression expression);


char* generate_assembly_for_if_else_statement(struct If_Else statement);


char* generate_assembly_for_do_while_statement(struct Do_While statement);


char* generate_assembly_for_while_statement(struct While statement);


char* generate_assembly_for_for_statement(struct For statement);
