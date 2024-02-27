#include "lexer.h"
#include "../utils/vector.c"
#include "../utils.c"
#include "string.h"


struct Lexer init_lexer(char* content, unsigned long long content_length) {
    struct Lexer lexer = {};
    lexer.content = content;
    lexer.content_length = content_length;
    return lexer;
}


struct Vector* get_all_tokens(struct Lexer* lexer) {
    struct Vector* vector = init_vector(4, TOKEN);
    struct Token* current_token;
    unsigned long long position = 0;
    do {
        while (lexer->content[position] == ' ' || lexer->content[position] == '\t' || lexer->content[position] == '\n') {
            ++position;
        }
        current_token = get_token(lexer->content, position);
        position += (current_token->attributes->text_length);
        push_back_token(vector, current_token);
    }
    while ((current_token->type != END));
    return vector;
}


struct Token* get_token(char* content, unsigned long long position) {
    struct Token* token;
    if (content[position] <= 32) {
        token = (struct Token*)malloc(sizeof(struct Token));
        token->type = END;
        token->attributes = (struct Token_Attributes*)malloc(sizeof(struct Token_Attributes));
        return token;
    }

    if (48 <= content[position] && content[position] <= 57) { // Digits
        token = get_number_token(content, position);
    } else if ((65 <= content[position] && content[position] <= 90) || content[position] == '_') {
        token = get_identifier_token(content, position);
    } else if (97 <= content[position] && content[position] <= 122) {
        token = get_keyword_token(content, position);
        if (token->type == INCORRECT) {
            token = get_identifier_token(content, position);
        }
    } else {
        if (content[position] == '"') {
            token = get_string_literal_token(content, position);
        } else if (content[position] == '\'') {
            token = get_char_literal_token(content, position);
        } else {
            token = get_other_token(content, position);
        }
    }

    return token;
}


struct Token* get_number_token(char* content, unsigned long long position) {
    struct Token* token;

    if (content[position + 1] == 'x' || content[position + 1] == 'X') {
        token = determine_hexadecimal_token(content, position);
    } else if (content[position + 1] == 'o' || content[position + 1] == 'O') {
        token = determine_octal_token(content, position);
    } else if (content[position + 1] == 'b') {
        token = determine_binary_token(content, position);
    } else {
        token = determine_decimal_token(content, position);
    }

    return token;
}


struct Token* determine_hexadecimal_token(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    position += 2;
    while (
            (48 <= content[position] && content[position] <= 57)
            || (65 <= content[position] && content[position] <= 70)
            || (97 <= content[position] && content[position] <= 102)) {
        ++position;
    }
    char* lexeme_text = get_substring(content, start_position, position - 1);
    return init_token(
            HEXADECIMAL_INTEGER_LITERAL,
            lexeme_text,
            strlen(lexeme_text),
            0, // MUST BE IMPLEMENTED
            start_position);
}


struct Token* determine_octal_token(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    position += 2;
    while (48 <= content[position] && content[position] <= 55) {
        ++position;
    }
    char* lexeme_text = get_substring(content, start_position, position - 1);
    return init_token(
            OCTAL_INTEGER_LITERAL,
            lexeme_text,
            strlen(lexeme_text),
            0, // MUST BE IMPLEMENTED
            start_position);
}


struct Token* determine_binary_token(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    position += 2;
    while (content[position] == '0' || content[position] == '1') {
        ++position;
    }
    char* lexeme_text = get_substring(content, start_position, position - 1);
    return init_token(
            BINARY_INTEGER_LITERAL,
            lexeme_text,
            strlen(lexeme_text),
            0, // MUST BE IMPLEMENTED
            start_position);
}


struct Token* determine_decimal_token(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    position += 2;
    while (48 <= content[position] && content[position] <= 57) {
        ++position;
    }

    if (content[position] == '.') {
        return determine_real_token(content, start_position, position);
    } else if (content[position] == 'e' || content[position] == 'E') {
        // MUST BE IMPLEMENTED!
        return determine_exponential_real_token(content, position);
    } else {
        char* lexeme_text = get_substring(content, start_position, position - 1);
        return init_token(
                DECIMAL_INTEGER_LITERAL,
                lexeme_text,
                strlen(lexeme_text),
                0, // MUST BE IMPLEMENTED
                start_position);
    }
}


struct Token* determine_real_token(char* content, unsigned long long start_position, unsigned long long position) {
    position += 2;
    while (48 <= content[position] && content[position] <= 57) {
        ++position;
    }
    char* lexeme_text = get_substring(content, start_position, position - 1);
    return init_token(
            DECIMAL_REAL_LITERAL,
            lexeme_text,
            strlen(lexeme_text),
            0, // MUST BE IMPLEMENTED
            start_position);
}


struct Token* determine_exponential_real_token(char* content, unsigned long long position) {

}


struct Token* get_string_literal_token(char* content, unsigned long long position) {
    unsigned long long start_position = position++;
    while (content[position] != '"') {
        ++position;
    }
    ++position;
    char* lexeme_text = get_substring(content, start_position, position);
    return init_token(
            STRING_LITERAL,
            lexeme_text,
            strlen(lexeme_text),
            0, // MUST BE IMPLEMENTED
            start_position);
}


struct Token* get_char_literal_token(char* content, unsigned long long position)  {
    unsigned long long start_position = position++;
    while (content[position] != '\'') {
        ++position;
    }
    char* lexeme_text = get_substring(content, start_position, position);
    if (content[start_position + 1] == '\'' || content[start_position + 2] != '\'') {
        return init_token(
                INCORRECT,
                lexeme_text,
                strlen(lexeme_text),
                0,
                start_position
                );
    }
    return init_token(
            SYMBOL_LITERAL,
            lexeme_text,
            strlen(lexeme_text),
            0,
            start_position
            );
}


struct Token* get_keyword_token(char* content, unsigned long long position) {

}


struct Token* get_other_token(char* content, unsigned long long position) {

}


struct Token* get_identifier_token(char* content, unsigned long long position) {

}
