%{
    #include "stdio.h"
    #include "include/utils/vector.c"
    #include "string.h"

    enum Type_Of_Items type = (enum Type_Of_Items) TOKEN;
    struct Vector* vector;
%}

HEXADECIMAL_NUMBER_REGEX 0[xX][0-9a-fA-F]+

DECIMAL_REGEX [\d]+

OCTAL_NUMBER_REGEX 0[o][0-7]+

BINARY_NUMBER_REGEX 0[b][0-1]+

STRING_LITERAL_REGEX ["][x00-\x7F]+["]

CHAR_LITERAL_REGEX ['][x00-\x7F][']

HEXADECIMAL_REAL_NUMBER_REGEX 0[xX][0-9a-fA-F]+[.][0-9a-fA-F]+[pP][+|-][0-9]+

DECIMAL_REAL_NUMBER_REGEX [0-9]+[.][0-9]+

EXPONENTIAL_REAL_NUMBER_REGEX [0-9]+[.][0-9]+[eE][+|-][0-9]+

INTEGER_DATA_TYPE_REGEX char|short|int|long

REAL_DATA_TYPE_REGEX float|double

SIGNED_MODIFIER_REGEX signed|unsigned

LOOPS_REGEX do|while|for

CONDITIONAL_REGEX if|else|switch|case|default

TRANSITION_INTERRUPT_OPERATOR_REGEX continue|break|return|goto

COMPLEX_DATA_TYPE_REGEX struct|union|enum

OTHER_KEY_WORD_REGEX static|void|const|typedef|sizeof

ARITHMETIC_OPERATOR_REGEX \+|-|\*|\/|%|\++|--

COMPARISON_OPERATOR_REGEX <|>|<=|>=|==|!=

BITWISE_OPERATOR_REGEX <<|>>|&|\|~

ASSIGNMENT_OPERATOR_REGEX =

COMPLEX_ASSIGNMENT_OPERATOR_REGEX \+=|-=|\*=|%=|\/=|<<=|>>=|\|=|&=|~=

ROUND_BRACKET_REGEX \(|\)

SQUARE_BRACKET_REGEX \[|\]

CURCLY_BRACKET_REGEX \{|\}

TRIANGLE_BRACKET_REGEX \<|\>

%option noyywrap

%%
{HEXADECIMAL_NUMBER_REGEX}+ {
                            enum Type_Of_Token token_type = (enum Type_Of_Token)HEXADECIMAL_INTEGER_LITERAL;
                            push_back_token(vector, init_token(token_type, yytext, 1, 2, 3));
                            }
%%

int main()
{
    vector = init_vector(5, type);
    yyin = fopen("cmake-build-debug/test_case.c", "r");
    yylex();
    printf("%s!!!!\n", ((struct Token*)(vector->data[0]))->attributes->text);
    return 0;
}