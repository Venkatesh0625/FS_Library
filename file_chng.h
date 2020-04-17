#ifndef FILE_CHNG
#define FILE_CHNG

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "flags.h"
#include "change.h"

void copy_file(char* filename)
{
    char dest[24] = "temp/";
    strcat(dest,filename);
    char *argument[] = {"cp", filename, dest, NULL};
    execvp(argument[0],argument);
}

void update_file(struct change* chng)
{
    char sed_command[9030]= {0};
    
    printf("%s\n",sed_command);
    if(chng->type == INSERT_LINE) 
    {
        sprintf(sed_command, "%d i %s", chng->line_number, chng->line_data);
        char *argument[] = {"sed", "-i", sed_command, chng->filename, NULL};
        execvp(argument[0], argument);
    }
    
    else if(chng->type == REPLACE_LINE)
    {
        sprintf(sed_command, "%d s/.*/%s/", chng->line_number, chng->line_data);
        char *argument[] = {"sed", "-i", sed_command, chng->filename, NULL};
        execvp(argument[0], argument);
    }
    else if(chng->type == DELETE_LINE)
    {
        sprintf(sed_command, "%dd", chng->line_number);
        char *argument[] = {"sed", "-i", sed_command, chng->filename, NULL};
        execvp(argument[0], argument);
    }

    else if(chng->type == CREATE_FILE)
    {
        char *argument[] = {"touch", chng->filename};
        execvp(argument[0], argument);
    }
    else if(chng->type == DELETE_FILE)
    {
        char *argument[] = {"rm", "-f", chng->filename};
        execvp(argument[0], argument);
    }

}

void replace_file(char* filename, char* buffer)
{
    FILE* filePtr = fopen(filename,"w");
    fprintf(filePtr, "%s", buffer);
    fclose(filePtr);
}

#endif
