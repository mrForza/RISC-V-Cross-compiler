#include "file_handler.h"
#include "file_validator.h"
#include "stdlib.h"
#include "string.h"

#define MAX_FILENAME_LENGTH 256

const char* GENERAL_FILE_ERROR = "You have input the incorrect filename";


char* get_full_name() {
    char* file_name = (char*)malloc(MAX_FILENAME_LENGTH);
    char* result_line = file_name;
    int last = 0;
    while (true) {
        printf("\n\nEnter the name of your source file: ");
        for (int i = 0; i < MAX_FILENAME_LENGTH; ++i) {
            int ascii_code = fgetc(stdin);
            if (ascii_code == EOF || ascii_code == '\n') {
                break;
            }
            file_name[i] = (char)ascii_code;
            last = i + 1;
        }
        file_name[last] = '\0';
        printf("\n");
        return result_line;
    }
}


char** split_full_name(const char* full_name) {
    int dot_index = 0;
    for (int i = 0; i < strlen(full_name); ++i) {
        if (full_name[i] == '.') {
            dot_index = i;
            break;
        }
    }

    char* name = (char*)malloc(dot_index + 1); // +1 for \0 symbol in the end of name
    char* extension = (char*)malloc(strlen(full_name) - dot_index);
    int j = 0;
    for (int i = 0; i < strlen(full_name); ++i) {
        if (i < dot_index) {
            name[i] = full_name[i];
        } else if (i > dot_index) {
            extension[j] = full_name[i];
            ++j;
        } else {
            name[dot_index] = '\0';
        }

        if (i == strlen(full_name) - 1) {
            extension[j] = '\0';
        }
    }

    char** array = (char**)malloc(strlen(name) + strlen(extension));
    array[0] = name;
    array[1] = extension;
    return array;
}