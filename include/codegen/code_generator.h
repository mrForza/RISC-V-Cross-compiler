char* generate_assembly_for_var_declaration(char* type, char* var_name);


char* generate_assembly_for_variables_declarations(char* type, char** var_names, unsigned long long count);


char* generate_assembly_for_var_definition(char* type, char* var_name, char* value);


char* generate_assembly_for_variables_definitions(char* type, char** var_names, char** values, unsigned long long count);


char* create_variable_alias(char* varname, int shift);


char* generate_assembly_for_arithmetic_expression(struct Arithmetic_Expression expression);
