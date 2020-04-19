#ifndef ERROR_H
#define ERROR_H

#include<stdio.h>

void throw_error(char* err)
{
    perror(message);
    exit(EXIT_FAILURE);
}

#endif