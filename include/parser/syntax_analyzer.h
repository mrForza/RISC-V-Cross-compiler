int find_close_curly_bracket(void** tokens, int start_position, int size);


int find_nearest_semicolon(void** tokens, int start_position, int size);


int find_close_bracket(void** tokens, int start_position, int size);


bool is_arithmetic_expression(void** tokens, const int* start_position, int size);


bool is_single_declaration_expression(void** tokens, const int* start_position, int size);


bool is_complex_declaration_expression(void** tokens, const int* start_position, int size);


bool is_single_definition_expression(void** tokens, const int* start_position, int size);


bool is_complex_definition_expression(void** tokens, const int* start_position, int size);


bool is_logical_expression(void** tokens, const int* start_position, int size);


bool is_relational_expression(void** tokens, const int* start_position, int size);


bool is_bitwise_expression(void** tokens, const int* start_position, int size);


bool is_if_else_statement(void** tokens, const int* start_position, int size);


bool is_while_statement(void** tokens, const int* start_position, int size);


bool is_do_while_statement(void** tokens, const int* start_position, int size);


bool is_for_statement(void** tokens, const int* start_position, int size);


bool is_prefix_expression(void** tokens, const int* start_position, int size);


bool is_postfix_expression(void** tokens, const int* start_position, int size);


bool is_function_declaration(void** tokens, const int* start_position, int end);


bool is_function_calling(void** tokens, const int* start_position, int end);