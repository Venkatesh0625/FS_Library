#ifndef CHANGE_H
#define CHANGE_H

#include <stdio.h>

struct change {
    int type;
    int line_number;
    char filename[100];
    char line_data[9016];
};

#endif