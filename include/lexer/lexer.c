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
    struct Vector* vector = init_vector(1024, TOKEN);
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


struct Token* get_semicolon_token(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    char* lexeme_text = get_substring(content, start_position, start_position);
    return init_token(
            SEMICOLON,
            lexeme_text,
            strlen(lexeme_text),
            0, // MUST BE IMPLEMENTED
            start_position);
}


struct Token* get_token(char* content, unsigned long long position) {
    struct Token *token;
    if (content[position] < 32 && content[position] != 10 && content[position] != 9) {
        token = (struct Token *) malloc(sizeof(struct Token));
        token->type = END;
        token->attributes = (struct Token_Attributes *) malloc(sizeof(struct Token_Attributes));
        return token;
    }

    if (content[position] == ';') {
        token = get_semicolon_token(content, position);
        goto _end;
    }

    if (48 <= content[position] && content[position] <= 57) {
        token = get_number_token(content, position);
    } else if ((65 <= content[position] && content[position] <= 90) || content[position] == '_') {
        token = get_identifier_token(content, position);
    } else if (97 <= content[position] && content[position] <= 122) {
        token = get_keyword_token(content, position);
    } else {
        if (content[position] == '"') {
            token = get_string_literal_token(content, position);
        } else if (content[position] == '\'') {
            token = get_char_literal_token(content, position);
        } else {
            token = get_other_token(content, position);
        }
    }

_end:
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
    while ((48 <= content[position] && content[position] <= 55) ||
            (65 <= content[position] && content[position] <= 70) ||
            (97 <= content[position] && content[position] <= 102)) {
        ++position;
    }
    char* lexeme_text = get_substring(content, start_position, position - 1);
    return init_token(
            HEX_INT_LITERAL,
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
            OCTAL_INT_LITERAL,
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
            BINARY_INT_LITERAL,
            lexeme_text,
            strlen(lexeme_text),
            0, // MUST BE IMPLEMENTED
            start_position);
}


struct Token* determine_decimal_token(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    position += 1; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    while (48 <= content[position] && content[position] <= 57) {
        ++position;
    }

    if (content[position] == '.') {
        return determine_real_token(content, start_position, position);
    } else {
        char* lexeme_text = get_substring(content, start_position, position - 1);
        return init_token(
                DECIMAL_INT_LITERAL,
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
            DOUBLE_LITERAL,
            lexeme_text,
            strlen(lexeme_text),
            0, // MUST BE IMPLEMENTED
            start_position);
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
            CHAR_LITERAL,
            lexeme_text,
            strlen(lexeme_text),
            0,
            start_position
            );
}


struct Token* get_current_keyword(char* content, unsigned long long position, const char* keyword,
        enum Type_Of_Token type) {
    char* substring = get_substring(content, position, position + strlen(keyword) - 1);
    unsigned long long start_position = position;
    bool is_keyword = true;

    for (unsigned long long i = 0; i < strlen(keyword); ++i) {
        if (substring[i] != keyword[i]) {
            is_keyword = false;
            break;
        }
    }

    if (is_keyword) {
        return init_token(
                type,
                substring,
                strlen(substring),
                0,
                start_position);
    }

    return init_token(INCORRECT, "", 0, 0, 0);
}


struct Token* get_b_start_keyword_token(char* content, unsigned long long position) {
    char* substring = get_substring(content, position, position + 4);
    unsigned long long start_position = position;
    char* keyword_break = "break";
    bool is_keyword = true;

    for (unsigned long long i = 0; i < 5; ++i) {
        if (substring[i] != keyword_break[i]) {
            is_keyword = false;
            break;
        }
    }

    if (is_keyword) {
        return init_token(
                BREAK,
                substring,
                strlen(substring),
                0,
                start_position);
    }

    return get_identifier_token(content, start_position);
}


struct Token* get_c_start_keyword_token(char* content, unsigned long long position) {
    struct Token* token;
    unsigned long long start_position = position;

    token = get_current_keyword(content, start_position, "char", CHAR);
    if (token->type != INCORRECT) {
        return token;
    }

    token = get_current_keyword(content, start_position, "case", CASE);
    if (token->type != INCORRECT) {
        return token;
    }

    token = get_current_keyword(content, start_position, "const", CONST);
    if (token->type != INCORRECT) {
        return token;
    }

    token = get_current_keyword(content, start_position, "continue", CONTINUE);
    if (token->type != INCORRECT) {
        return token;
    }

    return get_identifier_token(content, position);
}


struct Token* get_do_keyword(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    char* substring = get_substring(content, position, position + 1);
    char* keyword = "do";
    bool is_keyword = true;

    for (unsigned long long i = 0; i < 2; ++i) {
        if (substring[i] != keyword[i]) {
            is_keyword = false;
            break;
        }
    }

    char sym = content[2];
    if (is_keyword && content[start_position + 2] != 'u') {
        return init_token(
                DO,
                substring,
                strlen(substring),
                0,
                start_position);
    }

    return get_identifier_token(content, start_position);
}


struct Token* get_double_keyword(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    char* substring = get_substring(content, position, position + 5);
    char* keyword = "double";
    bool is_keyword = true;

    for (unsigned long long i = 0; i < 6; ++i) {
        if (substring[i] != keyword[i]) {
            is_keyword = false;
            break;
        }
    }

    if (is_keyword) {
        return init_token(
                DOUBLE,
                substring,
                strlen(substring),
                0,
                start_position);
    }

    return get_identifier_token(content, start_position);
}


struct Token* get_default_keyword(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    char* substring = get_substring(content, position, position + 6);
    char* keyword = "default";
    bool is_keyword = true;

    for (unsigned long long i = 0; i < 7; ++i) {
        if (substring[i] != keyword[i]) {
            is_keyword = false;
            break;
        }
    }

    if (is_keyword) {
        return init_token(
                DEFAULT,
                substring,
                strlen(substring),
                0,
                start_position);
    }

    return get_identifier_token(content, start_position);
}


struct Token* get_d_start_keyword_token(char* content, unsigned long long position) {
    struct Token* token;
    unsigned long long start_position = position;

    token = get_do_keyword(content, start_position);
    if (token->type != INCORRECT) {
        return token;
    }

    token = get_double_keyword(content, start_position);
    if (token->type != INCORRECT) {
        return token;
    }

    token = get_default_keyword(content, start_position);
    if (token->type != INCORRECT) {
        return token;
    }

    return get_identifier_token(content, position);
}


struct Token* get_e_start_keyword_token(char* content, unsigned long long position) {
    struct Token* token;
    unsigned long long start_position = position;

    token = get_current_keyword(content, start_position, "else", ELSE);
    if (token->type != INCORRECT) {
        return token;
    }

    return get_identifier_token(content, position);
}


struct Token* get_f_start_keyword_token(char* content, unsigned long long position) {
    struct Token* token;
    unsigned long long start_position = position;

    token = get_current_keyword(content, start_position, "for", FOR);
    if (token->type != INCORRECT) {
        return token;
    }

    return get_identifier_token(content, position);
}


struct Token* get_i_start_keyword_token(char* content, unsigned long long position) {
    struct Token* token;
    unsigned long long start_position = position;

    token = get_current_keyword(content, position, "if", IF);
    if (token->type != INCORRECT) {
        return token;
    }

    token = get_current_keyword(content, position, "int", INT);
    if (token->type != INCORRECT) {
        return token;
    }

    return get_identifier_token(content, position);
}


struct Token* get_return_keyword(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    char* substring = get_substring(content, position, position + 5);
    char* keyword = "return";
    bool is_keyword = true;

    for (unsigned long long i = 0; i < 6; ++i) {
        if (substring[i] != keyword[i]) {
            is_keyword = false;
            break;
        }
    }

    if (is_keyword) {
        return init_token(
                RETURN,
                substring,
                strlen(substring),
                0,
                start_position);
    }

    return get_identifier_token(content, start_position);
}


struct Token* get_void_keyword(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    char* substring = get_substring(content, position, position + 3);
    char* keyword = "void";
    bool is_keyword = true;

    for (unsigned long long i = 0; i < 4; ++i) {
        if (substring[i] != keyword[i]) {
            is_keyword = false;
            break;
        }
    }

    if (is_keyword) {
        return init_token(
                VOID,
                substring,
                strlen(substring),
                0,
                start_position);
    }

    return get_identifier_token(content, start_position);
}


struct Token* get_while_keyword(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    char* substring = get_substring(content, position, position + 4);
    char* keyword = "while";
    bool is_keyword = true;

    for (unsigned long long i = 0; i < 5; ++i) {
        if (substring[i] != keyword[i]) {
            is_keyword = false;
            break;
        }
    }

    if (is_keyword) {
        return init_token(
                WHILE,
                substring,
                strlen(substring),
                0,
                start_position);
    }

    return get_identifier_token(content, start_position);
}


struct Token* get_s_start_keyword_token(char* content, unsigned long long position) {
    struct Token* token;
    unsigned long long start_position = position;

    token = get_current_keyword(content, position, "switch", SWITCH);
    if (token->type != INCORRECT) {
        return token;
    }

    return get_identifier_token(content, position);
}


struct Token* get_keyword_token(char* content, unsigned long long position) {
    char first_symbol = content[position];

    switch (first_symbol) {
        case 'b':
            return get_b_start_keyword_token(content, position);
        case 'c':
            return get_c_start_keyword_token(content, position);
        case 'd':
            return get_d_start_keyword_token(content, position);
        case 'e':
            return get_e_start_keyword_token(content, position);
        case 'f':
            return get_f_start_keyword_token(content, position);
        case 'i':
            return get_i_start_keyword_token(content, position);
        case 'r':
            return get_return_keyword(content, position);
        case 's':
            return get_s_start_keyword_token(content, position);
        case 'v':
            return get_void_keyword(content, position);
        case 'w':
            return get_while_keyword(content, position);
        default:
            return get_identifier_token(content, position);
    }
}


struct Token* get_brackets_token(char* content, unsigned long long position) {
    enum Type_Of_Token type;
    switch (content[position++]) {
        case '(':
            type = LEFT_ROUND_BRACKET;
            break;
        case ')':
            type = RIGHT_ROUND_BRACKET;
            break;
        case '[':
            type = LEFT_SQUARE_BRACKET;
            break;
        case ']':
            type = RIGHT_SQUARE_BRACKET;
            break;
        case '{':
            type = LEFT_CURLY_BRACKET;
            break;
        case '}':
            type = RIGHT_CURLY_BRACKET;
            break;
        default:
            type = INCORRECT;
            break;
    }
    char* lexeme_text = get_substring(content, position - 1, position - 1);
    return init_token(type, lexeme_text, 1, 0, position);
}


struct Token* get_assignment_token(char* content, unsigned long long position) {
    enum Type_Of_Token type = ASSIGN_OPERATOR;
    char symbols[] = {'+', '-', '*', '/', '%', '|', '&', '~'};
    bool is_correct = false;

    if (content[position + 1] == '=') {
        for (int i = 0; i < 8; ++i) {
            if (content[position] == symbols[i]) {
                is_correct = true;
            }
        }

        if (is_correct) {
            type = ARITHMETIC_ASSIGN_OPERATOR;
        } else {
            type = INCORRECT;
        }
    } else {
        if (content[position] == '=') {
            type = ASSIGN_OPERATOR;
            char* lexeme_text = get_substring(content, position, position);
            return init_token(type, lexeme_text, 1, 0, position);
        }
    }
    char* lexeme_text = get_substring(content, position, position + 1);
    return init_token(type, lexeme_text, 2, 0, position);
}


struct Token* get_bitwise_token(char* content, size_t position) {
    enum Type_Of_Token type;
    char symbols[] = {'|', '&', '~'};
    bool is_correct = false;
    for (int i = 0; i < 3; ++i) {
        if (content[position] == symbols[i]) {
            is_correct = true;
        }
    }
    if (is_correct) {
        type = BITWISE_OPERATOR;
    } else {
        type = INCORRECT;
    }
    char* lexeme_text = get_substring(content, position, position);
    return init_token(type, lexeme_text, 1, 0, position);
}


struct Token* get_arithmetic_token(char* content, unsigned long long position) {
    enum Type_Of_Token type;
    char symbols[] = {'+', '-', '*', '/', '%'};
    bool is_correct = false;
    for (int i = 0; i < 8; ++i) {
        if (content[position] == symbols[i]) {
            is_correct = true;
        }
    }
    if (is_correct) {
        type = ARITHMETIC_OPERATOR;
    } else {
        if ((content[position] == '+' && content[position + 1] == '+')
            || (content[position] == '-' && content[position + 1] == '-')) {
            type = ARITHMETIC_OPERATOR;
            char* lexeme_text = get_substring(content, position, position + 1);
            return init_token(type, lexeme_text, 2, 0, position);
        }
        type = INCORRECT;
    }
    char* lexeme_text = get_substring(content, position, position);
    return init_token(type, lexeme_text, 1, 0, position);
}


struct Token* get_comparison_token(char* content, unsigned long long position) {
    enum Type_Of_Token type;
    if (content[position] == '<' || content[position] == '>') {
        type = COMPARISON_OPERATOR;
    } else {
        if ((content[position] == '=' || content[position] == '!' || content[position] == '<' || content[position] == '>')
            && content[position + 1] == '=') {
            type = COMPARISON_OPERATOR;
        } else {
            type = INCORRECT;
        }
    }

    char* lexeme_text = get_substring(content, position, position + 1);
    return init_token(type, lexeme_text, 2, 0, position);
}


struct Token* get_logic_token(char* content, unsigned long long position) {
    enum Type_Of_Token type;
    if ((content[position] == '&' && content[position + 1] == '&')
        || (content[position] == '|' && content[position + 1] == '|')) {
        type = LOGIC_OPERATOR;
    } else {
        type = INCORRECT;
    }

    char* lexeme_text = get_substring(content, position, position + 1);
    return init_token(type, lexeme_text, 2, 0, position);
}


struct Token* get_other_token(char* content, unsigned long long position) {
    struct Token* token;

    token = get_brackets_token(content, position);
    if (token->type != INCORRECT) {
        return token;
    }

    token = get_logic_token(content, position);
    if (token->type != INCORRECT) {
        return token;
    }

    token = get_comparison_token(content, position);
    if (token->type != INCORRECT) {
        return token;
    }

    token = get_bitwise_token(content, position);
    if (token->type != INCORRECT) {
        return token;
    }

    token = get_arithmetic_token(content, position);
    if (token->type != INCORRECT) {
        return token;
    }

    return get_assignment_token(content, position);
}


struct Token* get_identifier_token(char* content, unsigned long long position) {
    unsigned long long start_position = position;
    position += 1;
    while (
            (48 <= content[position] && content[position] <= 57)
            || (65 <= content[position] && content[position] <= 90)
            || (97 <= content[position] && content[position] <= 122)
            || (content[position] == '_')) {
        ++position;
    }
    char* lexeme_text = get_substring(content, start_position, position - 1);
    return init_token(
            IDENTIFIER,
            lexeme_text,
            strlen(lexeme_text),
            0, // MUST BE IMPLEMENTED
            start_position);
}
