#ifndef CHANGE_H
#define CHANGE_H

#include <iostream>

#define CREATE_FILE 1
#define DELETE_FILE 2
#define INSERT_LINE 3 
#define REPLACE_LINE 4 
#define DELETE_LINE 5 
#define REPLACE_FILE 6
#define CREATE_N_REPLACE 7

struct change {
    int type;
    int line_number;
    char filename[100];
    char line_data[916];
};

#endif