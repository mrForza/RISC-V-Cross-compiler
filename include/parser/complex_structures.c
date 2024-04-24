#pragma once
#include "stdbool.h"

enum Type_Of_Grammars {
    SINGLE_DECLARATION = 0,
    COMPLEX_DECLARATION = 1,
    SINGLE_DEFINITION = 2,
    COMPLEX_DEFINITION = 3,
    ARITHMETIC_EXPRESSION = 4,
    LOGIC_EXPRESSION = 5,
    RELATIONAL_EXPRESSION = 6,
    IF_ELSE_STATEMENT = 7,
    DO_WHILE_STATEMENT = 8,
    WHILE_STATEMENT = 9,
    FOR_STATEMENT = 10,
    FUNCTION_DECLARATION = 11,
    ASSIGNMENT_EXPRESSION = 12,
    GENERAL_ASSIGNMENT_EXPRESSION = 13,
    READ_INT = 14,
    WRITE_INT = 15,
    READ_FLOAT = 16,
    WRITE_FLOAT = 17,
    UNKNOWN = 18
};


struct Grammar {
    void* data;
    enum Type_Of_Grammars type;
};


struct Arithmetic_Expression {
    void* left_operand;
    char* operator; // ARITHMETIC_OPERATOR OR BITWISE_OPERATOR
    void* right_operand;
    bool is_left_expr;
    bool is_right_expr;
};


struct Logic_Expression {
    void* left_operand;
    char* operator; // LOGIC_OPERATOR
    void* right_operand;
    bool is_left_expr;
    bool is_right_expr;
};


struct Relational_Expression {
    void *left_operand;
    char* operator; // RELATIONAL_OPERATOR
    void *right_operand;
    bool is_left_expr;
    bool is_right_expr;
};


struct Single_Declaration {
    char* type;
    char* var_name;
};


struct Complex_Declaration {
    char* type;
    char** var_names;
    int quantity_of_variables;
};


struct Single_Definition {
    char* type;
    char* var_name;
    void* expression;
    bool is_expression;
    bool is_variable;
};


struct Complex_Definition {
    char* type;
    char** var_names;
    void** expressions;
    int quantity_of_variables;
};


struct Assignment_Expression {
    char* var_name;
    char* sign;
    void* value;
    bool is_value_expression;
    bool is_variable;
};


struct General_Assignment_Expression {
    char* var_name;
    void* value;
    bool is_value_expression;
    bool is_variable;
};


struct If_Else {
    struct Grammar* conditions; // n
    struct Grammar** bodies; // n + 1
    int* grammars_quantities;
    int* quantities_of_variables;
    int quantity_of_conditions;
    int quantity_of_bodies;
    bool is_else_exists;
};


struct While {
    struct Grammar condition;
    struct Grammar* body;
    int grammars_quantity;
    int quantity_of_variables;
};


struct Do_While {
    struct Grammar condition;
    struct Grammar* body;
    int grammars_quantity;
    int quantity_of_variables;
};


struct For {
    struct Single_Definition definition;
    struct Relational_Expression relation;
    struct General_Assignment_Expression expr;
    struct Grammar* body;
    int quantity;
};


struct Read_Int_Function {
    char* var_name;
};


struct Write_Int_Function {
    char* value;
    bool is_var;
};


struct Read_Float_Function {
    char* var_name;
};


struct Write_Float_Function {
    char* value;
    bool is_var;
};


struct Unknown {

};
