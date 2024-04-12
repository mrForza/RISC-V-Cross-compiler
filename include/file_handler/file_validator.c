#include "file_validator.h"
#define COUNT_OF_FORBIDDEN_SYMBOLS 10

const char* FORBIDDEN_SYMBOL_ERROR = "Your file's name consist forbidden symbol!\n"
                                     "You musn't use \\ / : * ? \" < > | + symbols\t";

const char* NO_NAME_ERROR = "There is no name in your file!\t";

const char* NO_EXTENSION_ERROR = "There is no extension in your file!\t";

const char* REPEATED_DOT_ERROR = "You have an incorrect structure of filename\t";

const char* FILE_DOES_NOT_EXIST = "There is no source file in current directory!";

const char* INCORRECT_EXTENSION_ERROR = "Your source file has an incorrect extension";

const char FORBIDDEN_SYMBOLS[COUNT_OF_FORBIDDEN_SYMBOLS] = {
    '/',
    '\\',
    ':',
    '*',
    '?',
    '"',
    '<',
    '>',
    '|',
    '+'
};

char AVAILABLE_EXTENSIONS[2] = {
    'c', // source file
    'h' // header file
};


bool validate_filename(const char* filename) {
    bool dot_existance = false;
    unsigned long long dot_index = -1;
    unsigned long long index = 0;

    while (*filename != '\0') {
        for (int i = 0; i < COUNT_OF_FORBIDDEN_SYMBOLS; ++i) {
            if (*filename == FORBIDDEN_SYMBOLS[i]) {
                printf("%s", FORBIDDEN_SYMBOL_ERROR);
                return false;
            }
        }
        if (*filename == '.') {
            if (!dot_existance) {
                dot_index = index;
                dot_existance = true;
            } else {
                printf("%s", REPEATED_DOT_ERROR);
                return false;
            }
        }
        ++filename;
        ++index;
    }

    if (dot_index == 0) {
        printf("%s", NO_NAME_ERROR);
        return false;
    } else if (dot_index == index || !dot_existance) {
        printf("%s", NO_EXTENSION_ERROR);
        return false;
    }
    return true;
}


bool validate_extension(const char* extension) {
    unsigned long long extension_length = 0;
    while (*extension != '\0') {
        if (extension_length > 1) {
            printf("%s", INCORRECT_EXTENSION_ERROR);
            return false;
        }
        ++extension;
        ++extension_length;
    }
    extension -= extension_length;
    if (*extension != AVAILABLE_EXTENSIONS[0] && *extension != AVAILABLE_EXTENSIONS[1]) {
        printf("%s", INCORRECT_EXTENSION_ERROR);
        return false;
    }
    return true;
}


bool validate_file_existance(FILE* file) {
    if (file == NULL) {
        printf("%s", FILE_DOES_NOT_EXIST);
        return false;
    }
    return true;
}
