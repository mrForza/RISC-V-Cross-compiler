#include "stdio.h"


struct file_handler {
    FILE* file_pointer;
    const char* name;
    const char* extension;
    unsigned long long size;
};


char* get_full_name();


char** split_full_name(const char* full_name);
