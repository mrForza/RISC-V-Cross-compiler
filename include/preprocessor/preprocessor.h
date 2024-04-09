#include "stdio.h"
#include "stdbool.h"
#include "../file_handler/file_validator.h"


bool check_singleline_comment_existance(FILE* file_pointer);


bool check_multiline_comment_existance(FILE* file_pointer);


void delete_multiline_comments(struct file_handler handler);


void delete_singleline_comments(struct file_handler handler);


void delete_repetitive_spaces(struct file_handler handler);
