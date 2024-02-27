#include "token.h"
#include "stdlib.h"


struct Token* init_token(enum Type_Of_Token type, char* text, unsigned long long text_length,
                         unsigned long long line, unsigned long long column) {
    struct Token_Attributes* attributes = (struct Token_Attributes*)malloc(sizeof(struct Token_Attributes));
    struct Token* token = (struct Token*)malloc(sizeof(struct Token));
    token->type = type;

    attributes->text = text;
    attributes->text_length = text_length;
    attributes->line = line;
    attributes->column = column;
    token->attributes = attributes;

    return token;
}
