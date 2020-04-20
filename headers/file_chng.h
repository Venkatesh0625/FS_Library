#ifndef FILE_CHNG
#define FILE_CHNG

#include <iostream>
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


#include "flags.h"
#include "change.h"

using namespace std;

void copy_file(char* src, char* dest)
{
    char *argument[] = {"cp", src, dest, NULL};
    if(!fork()) 
        execvp(argument[0],argument);
    else
        wait(NULL);
}

void load_diff(char* actual_file, char* temp_file, char* diff_file)
{
    char command[200] = {0};
    sprintf(command, "git diff --no-index %s %s > %s", temp_file, actual_file, diff_file);
    char *argument[] = {"/bin/sh", "-c" ,command, NULL};
    if(!fork())
        execvp(argument[0],argument);
    else
        wait(NULL);
    
}

void update_file(struct change* chng)
{
    char sed_command[9030]= {0};
    
    printf("%s\n",sed_command);

    if(chng->type == INSERT_LINE) 
    {
        sprintf(sed_command, "%d i %s", chng->line_number, chng->line_data);
        char *argument[] = {"sed", "-i", sed_command, chng->filename, NULL};
        if(!fork())
            execvp(argument[0], argument);
        else
            wait(NULL);
    }
    
    else if(chng->type == REPLACE_LINE)
    {
        sprintf(sed_command, "%d s/.*/%s/", chng->line_number, chng->line_data);
        char *argument[] = {"sed", "-i", sed_command, chng->filename, NULL};
        if(!fork())
            execvp(argument[0], argument);
        else
        wait(NULL);
    }

    else if(chng->type == DELETE_LINE)
    {
        sprintf(sed_command, "%dd", chng->line_number);
        char *argument[] = {"sed", "-i", sed_command, chng->filename, NULL};
        if(!fork())
            execvp(argument[0], argument);
        else
        wait(NULL);
    }

    else if(chng->type == CREATE_FILE)
    {
        char *argument[] = {"touch", chng->filename};
        if(!fork())
            execvp(argument[0], argument);
        else
        wait(NULL);
    }
    else if(chng->type == DELETE_FILE)
    {
        char *argument[] = {"rm", "-f", chng->filename};
        if(!fork())
            execvp(argument[0], argument);
        else
        wait(NULL);
    }

}

void replace_file(char* filename, char* buffer)
{
    FILE* filePtr = fopen(filename,"w");
    fprintf(filePtr, "%s", buffer);
    fclose(filePtr);
}

int handle_data(queue<struct change> &que)
{
	while(true)
	{
		while(que.empty());

		while(!que.empty())
		{
			struct change data = que.front();
			update_file(&data);
			que.pop();
		}
	}
}

// int main()
// {

// }

#endif
