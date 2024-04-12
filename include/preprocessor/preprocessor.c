#include "preprocessor.h"


bool check_singleline_comment_existance(FILE* file_pointer) {
    char next_sym = (char)fgetc(file_pointer);
    return (next_sym != EOF && next_sym == '/' ? true : false);
}


bool check_multiline_comment_existance(FILE* file_pointer) {
    char next_sym = (char)fgetc(file_pointer);
    return (next_sym != EOF && next_sym == '*' ? true : false);
}


void delete_singleline_comments(struct file_handler handler) {
    FILE* write_file_pointer = fopen("formatted.c", "w");
    fseek(handler.file_pointer, 0L, SEEK_SET);
    char prev_sym = (char)fgetc(handler.file_pointer);
    char current_sym = (char)fgetc(handler.file_pointer);
    bool is_a_signleline_comment = false;

    fputc(prev_sym, write_file_pointer);
    while (current_sym != EOF) {
        if (is_a_signleline_comment) {
            if (current_sym == '\n') {
                is_a_signleline_comment = false;
                fputc(current_sym, write_file_pointer);
            }
        } else {
            if (prev_sym == '/' && current_sym == '/') {
                is_a_signleline_comment = true;
            } else {
                if (current_sym == '/') {
                    bool key = check_singleline_comment_existance(handler.file_pointer);
                    fseek(handler.file_pointer, -1, SEEK_CUR);
                    prev_sym = current_sym;
                    current_sym = (char)fgetc(handler.file_pointer);
                    if (key) {
                        continue;
                    }
                    fputc(prev_sym, write_file_pointer);
                }
                fputc(current_sym, write_file_pointer);
            }
        }
        prev_sym = current_sym;
        current_sym = (char)fgetc(handler.file_pointer);
    }
    fclose(write_file_pointer);
}


void delete_multiline_comments(struct file_handler handler) {
    handler.file_pointer = fopen("formatted.c", "r");
    FILE* write_file_pointer = fopen("formatted_2.c", "w");
    fseek(handler.file_pointer, 0L, SEEK_SET);
    char prev_sym = (char)fgetc(handler.file_pointer);
    char current_sym = (char)fgetc(handler.file_pointer);
    bool is_a_mutliline_comment = false;

    fputc(prev_sym, write_file_pointer);
    while (current_sym != EOF) {
        if (is_a_mutliline_comment) {
            if (prev_sym == '*' && current_sym == '/') {
                is_a_mutliline_comment = false;
            }
        } else {
            if (prev_sym == '/' && current_sym == '*') {
                is_a_mutliline_comment = true;
            } else {
                if (current_sym == '/') {
                    bool key = check_multiline_comment_existance(handler.file_pointer);
                    fseek(handler.file_pointer, -1, SEEK_CUR);
                    prev_sym = current_sym;
                    current_sym = (char)fgetc(handler.file_pointer);
                    if (key) {
                        continue;
                    }
                    fputc(prev_sym, write_file_pointer);
                }
                fputc(current_sym, write_file_pointer);
            }
        }
        prev_sym = current_sym;
        current_sym = (char)fgetc(handler.file_pointer);
    }
    fclose(write_file_pointer);
    fclose(handler.file_pointer);
}


void delete_repetitive_spaces(struct file_handler handler) {
    handler.file_pointer = fopen("formatted_2.c", "r");
    FILE* write_file_pointer = fopen("formatted_3.c", "w");
    fseek(handler.file_pointer, 0L, SEEK_SET);
    char prev_sym = (char)fgetc(handler.file_pointer);
    char current_sym = (char)fgetc(handler.file_pointer);

    fputc(prev_sym, write_file_pointer);
    while (current_sym != EOF) {
        if (prev_sym == ' ' && current_sym == ' ') {
            current_sym = (char)fgetc(handler.file_pointer);
            continue;
        }
        fputc(current_sym, write_file_pointer);
        prev_sym = current_sym;
        current_sym = (char)fgetc(handler.file_pointer);
    }
    fclose(write_file_pointer);
    fclose(handler.file_pointer);
}