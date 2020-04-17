#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stdio.h>
#include <string.h>
#include "change.h"

int serialize_structure(unsigned char* buffer, struct change *value)
{
    int i;
    for(i = 1;i <= 4;i++) 
        buffer[i-1] = (value->type >> (32 - i*8));

    for(i = 1;i <= 4;i++)
        buffer[4+i] = (value->line_number >> (32 - i*8));

    for(i = 0;value->line_data[i];i++) 
        buffer[8+i] = value->line_data[i];

    buffer[strlen(value->line_data) + 8] = '\n';

    return strlen(value->line_data) + 8;
}

struct change* deserialize_structure(char *buffer)
{
    int i;
    struct change *value = (struct change*) malloc(sizeof(struct change));
    value->type = 0;
    value->line_number = 0;
    strcpy(value->line_data, "");
    
    for(i = 1;i <= 4;i++) 
    { 
        value->type += buffer[i-1];
        value->type << (32 - i*8);
    }
    
    for(i = 0;i < 4;i++) 
    {
        value->line_number += buffer[4+i];
        value->line_number << (32 - i*8);
    }
    
    for(int i=0;buffer[8+i] != '\n';i++)
        value->line_data[i] = buffer[8+i];

    return value;
}

#endif

