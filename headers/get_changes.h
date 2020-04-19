#ifndef GET_CHANGES_H
#define GET_CHANGES_H

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <queue>

#include "file_chng.h"
#include "change.h"
#include "flags.h"

using namespace std;

int getLine_(char* line)
{
    int i,j;
    char nbr[10];

    for(i = 4,j = 0;;i++)
        if(line[i] == ',')
            break;
        else 
        {
            nbr[j] = line[i];
            j++;
        }

    return atoi(nbr);
}

void get_changes(char* filename, queue<struct change> &que)
{ 

    char    temp_path[100] = ".temp_dir/",
            actual_path[100] = ".watch_dir/",
            diff_file[100] = ".diff_dir/";

    strcat(diff_file, filename);
    strcat(diff_file, ".diff");
    strcat(temp_path, filename);
    strcat(actual_path, filename);

    printf("\n\n%s %s %s\n\n", actual_path, temp_path, diff_file);

    //function from header file file_chng
    load_diff(actual_path, temp_path, diff_file);

    //Update the temp file 
    copy_file(actual_path, temp_path);
    //variables for loading and extracting files
    FILE *file_ptr = fopen(diff_file,"r");
    char *line = {0};
    size_t len = 0;
    ssize_t read;

    //Keep track of changes
    char prev, curr;
    struct change data;

    //line_count keep tracks of the old file stored in temp_dir
    int i,line_count = 0;

    //reading 1st four lines in git diff
    for(int i =0;i<4;i++)
        if((read = getline(&line, &len, file_ptr)) == -1)
            break;


    while((read = getdelim(&line, &len, (int)'\n', file_ptr)) != -1)
    {
        data.type = 0;
        strcpy(data.filename,filename);
        line_count++;
        line[read-1] = '\0';   
        curr = line[0];
        if(curr == '@') {
            line_count = getLine_(line) - 1;
        }

        else if(curr == '+')
            if(prev == '-') {
                data.type = REPLACE_LINE;
                data.line_number = --line_count;
                strcpy(data.line_data,line+1);
            }

            else {
                data.type = INSERT_LINE;
                data.line_number = --line_count + 1;
                strcpy(data.line_data,line+1);
            }

        else if(curr == ' ' || curr == '-')
            if(prev == '-') {
                data.type = DELETE_LINE;
                data.line_number = line_count - 1;
                strcpy(data.line_data,"delete");
            }
        
        if(data.type != 0)
            que.push(data);

        if(curr != '\\')
            prev = curr;
        else
            --line_count;

    }
    //closing diff file 
    fclose(file_ptr);
}

#endif

