#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "change.h"

int serialize_structure(char* buffer, struct change *value)
{
    int i,j;
    for(i = 1;i <= 4;i++) 
        buffer[i-1] = (value->type >> (32 - i*8));

    for(i = 1;i <= 4;i++)
        buffer[3+i] = (value->line_number >> (32 - i*8));

    for(i = 8;value->filename[i-8];i++)
        buffer[i] = value->filename[i-8];

    //just a delimitter
    buffer[i] = '?';
    i++;
    for(j = 0;value->line_data[j];j++,i++) 
        buffer[i] = value->line_data[j];

    //just a delimitter for line_data
    buffer[i]= '\n';

    return i+1;
}

struct change* deserialize_structure(char *buffer)
{
    int i,j;
    struct change *value = (struct change*) malloc(sizeof(struct change));
    value->type = 0;
    value->line_number = 0;
    strcpy(value->line_data, "");
    
    for(i = 1;i <= 4;i++) 
    { 
        value->type += buffer[i-1];
        value->type << (32 - i*8);
    }
    
    for(i = 1;i <= 4;i++) 
    {
        value->line_number += buffer[3+i];
        value->line_number << (32 - i*8);
    }

    for(i = 8;buffer[i] != '?';i++)
        value->filename[i-8] = buffer[i];
    
    i++;

    for(j = 0;buffer[i] != '\n';i++,j++)
        value->line_data[j] = buffer[i];
    struct change* y = value;
    return value;
}



